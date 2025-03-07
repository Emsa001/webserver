#include "Webserv.hpp"

std::string removeTrailingSlash(const std::string &path) {
    if (path.empty()) return path;
    std::string normalizedPath = path;
    while (!normalizedPath.empty() && normalizedPath[normalizedPath.size() - 1] == '/') {
        normalizedPath.erase(normalizedPath.size() - 1);
    }
    return normalizedPath;
}

const config_map* Server::findLocation(const std::string &path) {
    const config_array& locations = this->config->at("locations").getArray();
    StringVec pathSegments = split(removeTrailingSlash(path), '/');

    const config_map* bestMatch = NULL;

    for (config_array::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        const config_map* location = &it->getMap();

        bool exact = location->count("exact") ? location->at("exact").getBool() : false;
        StringVec locationSegments = split(removeTrailingSlash(location->at("path").getString()), '/');

        if (!exact) {
            // Non-exact match: check if locationSegments is a prefix of pathSegments
            if (pathSegments.size() >= locationSegments.size() &&
                std::equal(locationSegments.begin(), locationSegments.end(), pathSegments.begin())) {
                if (!bestMatch || locationSegments.size() > split(bestMatch->at("path").getString(), '/').size()) {
                    bestMatch = location; // Keep longest matching prefix
                }
            }

        } else if (locationSegments == pathSegments){
            return location;
        }
    }

    return bestMatch;
}


void Server::handleResponse(int client_sock, char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock);

    const config_map *location = this->findLocation(request.getURL()->getPath());

    if(location == NULL) {
        response.setBody("not_found.html");
        response.respond();
        return ;
    }

    const std::string &root = location->at("root");

    std::string fullPath = std::string(ROOT_DIR) + root;
    fullPath += request.getURL()->getPath().substr(location->at("path").getString().size());

    if(isDirectory(fullPath)) 
        fullPath += "/" + location->at("index").getString();

    response.setBody(fullPath);
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
