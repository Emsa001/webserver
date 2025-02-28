#include "Server.hpp"

bool deleteFile(const std::string &filePath) {
    return (remove(filePath.c_str()) == 0);
}

std::string Server::DELETE(const std::string &request) {
    int path_start = request.find("DELETE /") + 8;
    int path_end = request.find(" ", path_start);
    std::string path = request.substr(path_start, path_end - path_start);

    std::string filePath = "./uploads/" + path;
    if (deleteFile(filePath))
        return "HTTP/1.1 200 OK\r\n\r\nFile deleted successfully.";
    else
        return "HTTP/1.1 404 Not Found\r\n\r\nFile not found.";
}