#include "Server.hpp"

void Server::listener(int server_sock) {
    listen(server_sock, MAX_CLIENTS);
    this->setNonBlocking(server_sock);

    pollfd fd;
    fd.fd = server_sock;
    fd.events = POLLIN;
    fd.revents = 0;

    fds.push_back(fd);

    const int timeout_ms = 1000;

    while (!g_stop) {
        int ret = poll(fds.data(), fds.size(), timeout_ms);
        if (ret < 0) {
            perror("poll");
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].fd == server_sock && (fds[i].revents & POLLIN)) {
                this->acceptNewConnections(server_sock);
            } else if (fds[i].revents & POLLIN) {
                this->handleClientRead(i);
            }
        }

        this->checkIdleClients();
        Logger::info("Active clients: " + intToString(fds.size() - 1));
    }

    Logger::info("server: " + this->getServerName() + " stopped");
}

void Server::acceptNewConnections(int server_sock) {
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_sock, (sockaddr*)&client_addr, &client_len);

        if (client_fd < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) break;
            perror("accept");
            return;
        }

        Logger::clientConnect(client_fd);
        this->setNonBlocking(client_fd);

        pollfd fd;
        fd.fd = client_fd;
        fd.events = POLLIN;
        fd.revents = 0;
        fds.push_back(fd);

        client_timestamps[client_fd] = time(NULL);
    }
}

void Server::checkIdleClients() {
    time_t now = time(NULL);
    static const int IDLE_TIMEOUT = Config::getSafe(*(this->config), "keep_alive", 30).getInt();

    for (size_t i = 1; i < fds.size(); ++i) {
        int fd = fds[i].fd;

        if (now - client_timestamps[fd] > IDLE_TIMEOUT) {
            // std::cout << "Idle timeout, closing fd: " << fd << std::endl;
            Logger::clientIdle(fd);
            this->removeClient(i);
            --i;
        }
    }
}

void Server::removeClient(size_t index) {
    int fd = fds[index].fd;
    int fd_copy = fd;

    this->closeConnection(&fd);
    fds.erase(fds.begin() + index);
    client_timestamps.erase(fd_copy);
}

void Server::closeConnection(int* client_sock) {
    if (client_sock && *client_sock >= 0) {
        Logger::clientDisconnect(*client_sock);
        close(*client_sock);
        *client_sock = -1;
    }
}

