#include "Webserv.hpp"

const FileData Server::createFileData(const config_map *location, HttpRequest &request) const{
    const std::string &root = location->at("root");
    const std::string &locationPath = location->at("path").getString();
    const std::string &requestPath = request.getURL()->getPath();

    std::string fullPath = std::string(ROOT_DIR) + root;
    fullPath += requestPath.substr(locationPath.size());

    // std::cout << std::endl;
    // std::cout << "Full path: " << fullPath << std::endl;
    // std::cout << "Request Path: " << requestPath << std::endl;
    // std::cout << "Location Path: " << locationPath << std::endl;
    // std::cout << std::endl;

    if(fullPath[fullPath.size() - 1] == '/' && locationPath == requestPath)
        fullPath += "/" + location->at("index").getString();

    return getFileData(fullPath);
}

void Server::handleResponse(int client_sock, char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock);

    const config_map *location = this->findLocation(request.getURL()->getPath());

    if(location == NULL) {
        response.respondStatusPage(404);
        return ;
    }

    const FileData fileData = this->createFileData(location, request);

    if(this->isDirectoryListing(location, fileData))
        response.setListing(true);

    response.setBody(fileData);
    response.respond();
}

bool Server::handle_client(int client_sock) {
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
