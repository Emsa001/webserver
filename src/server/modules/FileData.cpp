#include "Webserv.hpp"

const FileData Server::createFileData(const config_map *location, HttpRequest &request) const{
    const std::string &root = location->at("root");
    const std::string &locationPath = location->at("path").getString();
    std::string requestPath = trimChar(request.getURL()->getPath(), '/');

    if(requestPath.empty()) requestPath = "/";

    std::cout << "requestPath: " << requestPath << std::endl;
    std::cout << "Location path: " << locationPath << std::endl;

    std::string fullPath = std::string(ROOT_DIR) + root;
    fullPath += requestPath.substr(locationPath.size());

    std::cout << std::endl;
    std::cout << "Full path: " << fullPath << std::endl;
    std::cout << "Request path: " << requestPath << std::endl;
    std::cout << std::endl;

    if(fullPath[fullPath.size() - 1] == '/' && locationPath == requestPath){
        std::string index = Config::getSafe(*location, "index", "").getString();
        if(!index.empty()) fullPath += "/" + index;
        // TODO: check why I need to check
    }

    return getFileData(fullPath);
}
