#include "Webserv.hpp"

const FileData Server::createFileData(const config_map *location, HttpRequest *request) const{
    const std::string &root = location->at("root");
    std::string locationPath = trimChar(location->at("path").getString(), '/');
    std::string requestPath = trimChar(request->getURL()->getPath(), '/');

    if(requestPath.empty()) requestPath = "/";
    if(locationPath.empty()) locationPath = "/";

    std::string fullPath = std::string(ROOT_DIR) + root;
    fullPath += requestPath.substr(locationPath.size());

    if(fullPath[fullPath.size() - 1] == '/' && locationPath == requestPath){
        std::string index = Config::getSafe(*location, "index", (std::string)"index.html").getString();
        fullPath += "/" + index;
    }

    std::cout << std::endl;
    std::cout << "Full path: " << fullPath << std::endl;
    std::cout << "Request path: " << requestPath << std::endl;
    std::cout << "Location path: " << locationPath << std::endl;
    std::cout << std::endl;
    return getFileData(fullPath);
}


