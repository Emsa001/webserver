#include "Server.hpp"

void Server::send_response(int client_sock, const std::string &path) {
    std::string full_path = std::string(ROOT_DIR) + path;
    if (!full_path.empty() && full_path[full_path.length() - 1] == '/') full_path += "index.html";
    
    std::string content = read_file(full_path);
    if (content.empty()) {
        std::string not_found = "HTTP/1.1 404 Not Found\r\n"
                                "Content-Length: 13\r\n"
                                "Connection: close\r\n"
                                "\r\n404 Not Found";
        send(client_sock, not_found.c_str(), not_found.size(), 0);
        return;
    }

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << get_mime_type(full_path) << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: keep-alive\r\n";  // <-- Keep the connection open
    response << "\r\n";
    response << content;

    send(client_sock, response.str().c_str(), response.str().size(), 0);
}
