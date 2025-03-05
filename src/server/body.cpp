#include "Server.hpp"

std::string Server::getBody(const std::string &path) {
    std::string content = read_file(path);
    if (content.empty()) {
        return "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>";
    }
    return content;
}