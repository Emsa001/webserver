#include "Webserv.hpp"

void Server::logs(HttpRequest &request) {
    std::cout
        << RED300
        << "Request: " RESET
        << request.getMethod() << " " 
        << request.getURL()->getPath() << std::endl
        << std::endl;
}

bool Server::handleResponse(int client_sock, const char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock);

    this->logs(request);

    if (request.getHeader("Connection") == "close") {
        response.setHeader("Connection", "close");
    } else {
        response.setHeader("Connection", "keep-alive");
    }

    const config_map *location = this->findLocation(request.getURL()->getPath());
    if (location == NULL) {
        response.respondStatusPage(404);
        return false;
    }

    FileData fileData = this->createFileData(location, request);
    if (!fileData.exists) {
        response.respondStatusPage(404);
    }else{
        response.setSettings(location);
        response.buildBody(fileData);
        response.respond();
    }

    this->client_timestamps[client_sock] = time(NULL);
    return true;
}

void Server::handleClientRead(size_t index) {
    int fd = this->fds[index].fd;
    char buffer[4096];
    int bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read <= 0) {
        std::cout << "Closing connection (recv <= 0): " << fd << std::endl;
        this->removeClient(index);
        return;
    }

    buffer[bytes_read] = '\0';

    if (this->handleResponse(fd, buffer) == false) {
        std::cout << "Closing connection (handleResponse == false): " << fd << std::endl;
        this->removeClient(index);
        return;
    }
}
