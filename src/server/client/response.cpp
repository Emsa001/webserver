#include "Webserv.hpp"

void Server::isValidMethod(HttpRequest *request, const config_map &location){
    std::string methods = Config::getSafe(location, "methods", (std::string)DEFAULT_METHODS).getString();
    if (methods.find(request->getMethod()) == std::string::npos) {
        throw HttpRequestException(405);
    }
}

bool Server::handleResponse(int client_sock, HttpRequest *request) {
    HttpResponse response(client_sock, request, this->config);
    std::string connectionHeader = "close";

    try{
        request->parse();
        connectionHeader = request->getHeader("Connection");

        if (connectionHeader == "close") {
            response.setHeader("Connection", "close");
        } else {
            response.setHeader("Connection", "keep-alive");
        }

        const config_map *location = this->findLocation(request->getURL()->getPath());
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
