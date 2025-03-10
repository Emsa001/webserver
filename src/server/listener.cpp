#include "Server.hpp"

/*

    This needs to be restructured.

    - Handle multiple clients without any blocking
    - Handle persistent connections
    - Handle idle connections
    - Handle timeouts
    - Handle disconnections


    (for tobi)

*/

void Server::listener(int server_sock) {
    listen(server_sock, MAX_CLIENTS);
    this->set_nonblocking(server_sock);

    std::vector<pollfd> fds;
    std::map<int, time_t> client_timestamps; // Track last activity time

    fds.push_back((pollfd){server_sock, POLLIN, 0});

    int timeout_ms = 5000;

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
                    // Accept new client
                    int client_sock = accept(server_sock, NULL, NULL);
                    if (client_sock >= 0) {
                        this->set_nonblocking(client_sock);
                        fds.push_back((pollfd){client_sock, POLLIN, 0}); // Only POLLIN
                        client_timestamps[client_sock] = now;
                        std::cout << "New client connected: " << client_sock << std::endl;
                    }
                } else {
                    if (!handleClient(fds[i].fd)) {
                        int closed_fd = fds[i].fd;
                        shutdown(closed_fd, SHUT_WR);
                        close(closed_fd);
                        fds.erase(fds.begin() + i);
                        client_timestamps.erase(closed_fd);
                        i--;
                    } else {
                        client_timestamps[fds[i].fd] = now;
                    }
                }
            }
        }

        // Cleanup idle connections
        for (size_t i = 1; i < fds.size(); i++) {
            int fd = fds[i].fd;
            if (now - client_timestamps[fd] > 10) { // 10 seconds timeout
                std::cout << "Closing idle connection: " << fd << std::endl;
                shutdown(fd, SHUT_WR);
                close(fd);
                fds.erase(fds.begin() + i);
                client_timestamps.erase(fd);
                i--;
            }
        }
    }
}
