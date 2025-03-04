#include "Server.hpp"

std::string Server::get_response(Client &client) 
{
    std::string const &request = client.get_request();

    if (request.find("GET /") == 0) {
        return GET(request);
    } else if (request.find("POST /") == 0) {
        POST(request);
    } else if (request.find("DELETE /") == 0) {
        return DELETE(request);
    } else {
        return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
}
