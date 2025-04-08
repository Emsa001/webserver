#include "Webserv.hpp"

/*

    TODO: Maybe create a class for client?

*/

bool Server::handleResponse(int client_sock, const char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock, &request);

    std::string connectionHeader = request.getHeader("Connection");

    if (connectionHeader == "close") {
        response.setHeader("Connection", "close");
    } else {
        response.setHeader("Connection", "keep-alive");
    }

    const config_map *location = this->findLocation(request.getURL()->getPath());
    if (location == NULL) {
        response.respondStatusPage(404);
        return (connectionHeader != "close");
    }

    FileData fileData = this->createFileData(location, request);
    if (!fileData.exists) {
        response.respondStatusPage(404);
    } else {
        response.setSettings(location);
        response.buildBody(fileData);
        response.respond();
    }

    this->client_timestamps[client_sock] = time(NULL);
    return (connectionHeader != "close");
}


void Server::handleClientRead(size_t index) {
    int fd = this->fds[index].fd;
    char buffer[4096];

    while (true) {
        int bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_read < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                break;
            }
            std::cout << "Recv error on fd " << fd << ", closing connection." << std::endl;
            this->removeClient(index);
            return;
        } else if (bytes_read == 0) {
            std::cout << "Client disconnected (recv == 0): " << fd << std::endl;
            this->removeClient(index);
            return;
        }

        buffer[bytes_read] = '\0';

        if (!this->handleResponse(fd, buffer)) {
            std::cout << "Closing connection (handleResponse returned false): " << fd << std::endl;
            this->removeClient(index);
            return;
        }
    }
}
