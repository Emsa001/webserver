#include "Webserv.hpp"

void Server::handleResponse(int client_sock, const char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock);

    const config_map *location = this->findLocation(request.getURL()->getPath());

    if(location == NULL) {
        response.respondStatusPage(404);
        return;
    }
    
    std::cout << "Request: " << request.getMethod() << " " << request.getURL()->getPath() << std::endl;

    FileData fileData = this->createFileData(location, request);

    if (!fileData.exists) {
        response.respondStatusPage(404);
        return;
    }

    if(Config::getSafe(*location, "autoindex", false))
        response.setListing(true);
    if(Config::getSafe(*location, "cgi", false))
        response.setCGI(true);

    response.setBody(fileData);
    response.respond();
}

bool Server::handleClient(int client_sock) {
    if (client_sock == -1) {
        return false;
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return true;
        } else {
            std::cerr << "[ERROR] recv() failed on socket " << client_sock << ": " << strerror(errno) << ". Closing." << std::endl;
            this->closeConnection(&client_sock);

            return false;
        }
    }

    if (bytes_received == 0) {
        std::cout << "Client disconnected: " << client_sock << std::endl;
        this->closeConnection(&client_sock);
        return false;
    }

    this->handleResponse(client_sock, buffer);

    if (strstr(buffer, "Connection: close")) {
        std::cout << "Client requested to close connection: " << client_sock << std::endl;
        this->closeConnection(&client_sock);
        return false;
    }

    return true;
}
