#include "Webserv.hpp"

void Server::handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            std::cout << "Client disconnected: " << client_sock << std::endl;
            break;
        }

        std::istringstream request(buffer);
        std::string method, path;
        request >> method >> path;

        // handle response

        HttpResponse response(client_sock);
        response.setBody(this->getBody(path));
        response.respond(path);

        // handle connection close
        if (strstr(buffer, "Connection: close")) {
            std::cout << "Client requested to close connection: " << client_sock << std::endl;
            break;
        }
    }

    struct linger linger_opt = {1, 10};
    setsockopt(client_sock, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    close(client_sock);
    std::cout << "Closed connection with client " << client_sock << std::endl;
}
