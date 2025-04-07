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
    
    response.buildBody(fileData);
    response.respond();
}

int Server::receiveBytes(int client_sock, char *buffer) {
    int bytes_received = recv(client_sock, buffer, BUFFER_SIZE - 1, 0);

    if (bytes_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            throw std::runtime_error("recv() would block");

        std::cerr << "[ERROR] recv() failed on socket " << client_sock << ": " << strerror(errno) << ". Closing." << std::endl;
        this->closeConnection(&client_sock);
        throw std::runtime_error("recv() failed");
    }

    if (bytes_received == 0) {
        this->closeConnection(&client_sock);
        throw std::runtime_error("Connection closed by client");
    }

    return bytes_received;
}

void Server::closeConnectionRequest(int client_sock,char *buffer) {
    if (!strstr(buffer, "Connection: close")) return ;

    std::cout << "Client requested to close connection: " << client_sock << std::endl;
    this->closeConnection(&client_sock);
    throw std::runtime_error("Connection closed by client");
}

bool Server::handleClient(int client_sock) {
    if (client_sock == -1) return false;

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    while (true) {
        try{
            const int bytes_received = this->receiveBytes(client_sock, buffer);
            buffer[bytes_received] = '\0';

            this->handleResponse(client_sock, buffer);
            this->closeConnectionRequest(client_sock, buffer);
        }catch(const std::runtime_error &e){
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    return true;
}


