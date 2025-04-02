#include "Server.hpp"

void Server::listener(int server_sock) {
    listen(server_sock, MAX_CLIENTS);
    this->set_nonblocking(server_sock);


    fds.push_back((pollfd){server_sock, POLLIN, 0});

    int timeout_ms = 1000;

    while (true) {
        int ret = poll(&fds[0], fds.size(), timeout_ms);

        if (ret < 0) {
            std::cerr << "Poll error" << std::endl;
            break;
        }

        time_t now = time(NULL);

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_sock) {
                    int client_sock = accept(server_sock, NULL, NULL);
                    if (client_sock >= 0) {
                        this->set_nonblocking(client_sock);
                        fds.push_back((pollfd){client_sock, POLLIN, 0});
                        this->client_timestamps[client_sock] = now;
                        std::cout << "New client connected: " << client_sock << std::endl;
                    }
                    continue;
                } 

                if (handleClient(fds[i].fd)) {
                    this->client_timestamps[fds[i].fd] = now;
                }
            }
        }

        // std::cout <<  "Active clients: " << this->client_timestamps.size() << std::endl;
    }
}

void Server::closeConnection(int *client_sock) {
    if (*client_sock != -1) {
        shutdown(*client_sock, SHUT_WR);
        close(*client_sock);
        
        std::vector<pollfd>::iterator it = fds.begin();
        while (it != fds.end()) {
            if (it->fd == *client_sock) {
                fds.erase(it);
                break;
            }
            it++;
        }

        this->client_timestamps.erase(*client_sock);
        *client_sock = -1;
    }
}

