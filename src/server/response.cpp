#include "../../includes/server.hpp"

void POST(const std::string &request)
{
    size_t num = request.find("Content-Length: ");
    if (num == std::string::npos)
        return;

    int len = request.find("\r\n", num);
    std::string str = request.substr(num + 16, len - (num + 16));
    
    int full_content = std::atoi(str.c_str());
    int start = request.find("\r\n\r\n") + 4;
    std::string file_data = request.substr(start, full_content);

    std::ofstream file("./uploads/file.txt", std::ios::binary);
    if (!file)
        return;
        
    file.write(file_data.c_str(), file_data.size());
    file.close();
}

std::string DELETE(const std::string &request)
{
    int path_start = request.find("DELETE /") + 8;
    int path_end = request.find(" ", path_start);

    std::string path = request.substr(path_start, path_end - path_start);

    if (remove(path.c_str()) != 0)
        return "HTTP/1.1 404 Not Found\r\n\r\nResource not found.";

    return "HTTP/1.1 200 OK\r\n\r\nResource deleted successfully.";
}


std::string Server::get_response(Client &client) 
{
    if (client.request.find("GET /cgi-bin/hello.py") == 0)
    {
        std::string cgi_output = cgi_execute("./src/cgi/hello.py");
        if (cgi_output.empty())
            return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        return "HTTP/1.1 200 OK\r\n" + cgi_output;
    }

    if (client.request.find("GET /") == 0)
    {
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<!DOCTYPE html><html><head><title>Hello, World!</title></head>"
            "<body><h1>Hello, World!</h1></body></html>";
        return response;
    }

    if (client.request.find("POST /upload") == 0)
        POST(client.request);

    if (client.request.find("DELETE /") == 0)
        return DELETE(client.request);

    return "HTTP/1.1 400 Bad Request\r\n\r\n";
}

