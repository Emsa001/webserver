#include "Server.hpp"

void Server::listener(int server_sock){

    listen(server_sock, MAX_CLIENTS);
    this->set_nonblocking(server_sock);

    std::vector<pollfd> fds;
    fds.push_back((pollfd){server_sock, POLLIN, 0});

    while (true) {
        if (poll(&fds[0], fds.size(), -1) < 0) {
            std::cerr << "Poll error" << std::endl;
            break;
        }
        
        for (size_t i = 0; i < fds.size(); i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_sock) {
                    int client_sock = accept(server_sock, NULL, NULL);
                    if (client_sock >= 0) {
                        this->set_nonblocking(client_sock);
                        fds.push_back((pollfd){client_sock, POLLIN, 0});
                        std::cout << "New client connected from: " << client_sock << std::endl;
                    }
                } else {
                    handle_client(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    i--;
                }
            }
        }
    }
}