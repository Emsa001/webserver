#include "Server.hpp"

void Server::listener(int server_sock) {
    listen(server_sock, MAX_CLIENTS);
    this->set_nonblocking(server_sock);

    std::vector<pollfd> fds;
    std::map<int, time_t> client_timestamps; // Track last activity time

    fds.push_back((pollfd){server_sock, POLLIN, 0});

    while (true) {
        int timeout_ms = 1000; // Reduce timeout to 1s for better responsiveness
        int ret = poll(&fds[0], fds.size(), timeout_ms);

        if (ret < 0) {
            std::cerr << "Poll error" << std::endl;
            break;
        }

        time_t now = time(NULL);

        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_sock) {
                    // Accept new client
                    int client_sock = accept(server_sock, NULL, NULL);
                    if (client_sock >= 0) {
                        this->set_nonblocking(client_sock);
                        fds.push_back((pollfd){client_sock, POLLIN | POLLOUT, 0});
                        client_timestamps[client_sock] = now;
                        std::cout << "New client connected: " << client_sock << std::endl;
                    }
                } else {
                    // Handle client request immediately
                    if (!handle_client(fds[i].fd)) {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        client_timestamps.erase(fds[i].fd);
                        i--;
                    } else {
                        client_timestamps[fds[i].fd] = now; // Update activity time
                    }
                }
            }
        }

        // Cleanup idle connections
        for (size_t i = 1; i < fds.size(); i++) {
            if (now - client_timestamps[fds[i].fd] > 10) { // 10 seconds timeout
                std::cout << "Closing idle connection: " << fds[i].fd << std::endl;
                close(fds[i].fd);
                fds.erase(fds.begin() + i);
                client_timestamps.erase(fds[i].fd);
                i--;
            }
        }
    }
}

