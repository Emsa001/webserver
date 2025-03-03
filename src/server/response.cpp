#include "Server.hpp"

std::string Server::get_response(Client &client) {
    std::string const &request = client.get_request();

    if (request.find("GET /") == 0) {
        return GET(request);
    } 
    else if (request.find("POST /") == 0) {
        POST(request);
        return "HTTP/1.1 200 OK\r\n\r\nFile uploaded successfully.";
    } else if (request.find("DELETE /") == 0) {
        return DELETE(request);
    } else {
        return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
}
