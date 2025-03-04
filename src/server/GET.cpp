#include "Server.hpp"

std::string get_404() {
    std::ifstream file("./pages/404.html");
    if (!file) 
        return "HTTP/1.1 404 Not Found\r\n\r\n";

    std::string response =
    "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/html\r\n\r\n";

    std::string line;
    while (std::getline(file, line)) {
        response += line + "\n";
    }
    file.close();
    return response;
}

std::string Server::GET(const std::string &request) {
    int path_start = request.find("GET /") + 5;
    int path_end = request.find(" ", path_start);
    std::string path = request.substr(path_start, path_end - path_start);

    if (path == "") 
    {
        std::ifstream file("./pages/index.html");
        if (!file) 
            return "HTTP/1.1 404 Not Found\r\n\r\n";
        
        std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n";
        
        std::string line;
        while (std::getline(file, line)) {
            response += line + "\n";
        }
        file.close();
        return response;
    }
    else 
        return get_404();
}