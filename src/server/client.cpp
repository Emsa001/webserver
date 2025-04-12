#include "Webserv.hpp"

/*

    TODO: Maybe create a class for client?

*/

void Server::isValidMethod(HttpRequest &request, const config_map &location){
    const std::string defaultMethods = "GET POST DELETE";
    std::string availableMethods = Config::getSafe(location, "methods", defaultMethods);
    if (availableMethods.find(request.getMethod()) == std::string::npos) {
        throw HttpRequestException(405);
    }
}

bool Server::handleResponse(int client_sock, const char *buffer) {
    HttpRequest request(client_sock, buffer);
    HttpResponse response(client_sock, &request);

    request.setMaxHeaderSize(Config::getSafe(*this->config, "max_client_header_size", 8192));
    request.setMaxBodySize(Config::getSafe(*this->config, "max_client_body_size", 8192));
    std::string connectionHeader = "close";

    try{
        request.parse();
        connectionHeader = request.getHeader("Connection");

        if (connectionHeader == "close") {
            response.setHeader("Connection", "close");
        } else {
            response.setHeader("Connection", "keep-alive");
        }

        const config_map *location = this->findLocation(request.getURL()->getPath());
        if (location == NULL) {
            throw HttpRequestException(404);
        }

        this->isValidMethod(request, *location);

        FileData fileData = this->createFileData(location, request);
        if (!fileData.exists) {
            response.respondStatusPage(404);
        } else {
            response.setSettings(location);
            response.buildBody(fileData, request);
            response.respond();
        }
        this->client_timestamps[client_sock] = time(NULL);
        
    }catch(const HttpRequestException &e){
        response.respondStatusPage(e.getStatusCode());
        return (connectionHeader != "close");
    }

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
            this->removeClient(index);
            return;
        }
    }
}
