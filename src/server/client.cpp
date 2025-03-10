#include "Webserv.hpp"

void Server::handleResponse(int client_sock, char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock);

    const config_map *location = this->findLocation(request.getURL()->getPath());

    if(location == NULL) {
        response.respondStatusPage(404);
        return ;
    }
    
    std::cout << "Request: " << request.getMethod() << " " << request.getURL()->getPath() << std::endl;

    FileData fileData = this->createFileData(location, request);

    if(Config::getSafe(*location, "autoindex", false))
        response.setListing(true);

    response.setBody(fileData);
    response.respond();
}

bool Server::handleClient(int client_sock) {
    char buffer[BUFFER_SIZE];

    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

    if (bytes_received <= 0) {
        std::cout << "Client disconnected: " << client_sock << std::endl;
        return false; // Close connection
    }

    // Process the request
    this->handleResponse(client_sock, buffer);

    // Check for "Connection: close" header
    if (strstr(buffer, "Connection: close")) {
        std::cout << "Client requested to close connection: " << client_sock << std::endl;
        return false;
    }

    return true; // Keep connection alive
}
