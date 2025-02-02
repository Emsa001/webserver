#include "../../includes/webserv.hpp"
Webserver::Webserver(int port)
{
    // AF_UNIX, AF_LOCAL - Local communication
    // AF_INET - IPv4 Internet protocols
    // AF_INET6 - IPv6 Internet protocols
    // AF_IPX - IPX Novell protocols

    // SOCK_STREAM - Two-way reliable communication (TCP)
    // SOCK_DGRAM - Connectionless, unreliable (UDP)

    this->port = port; 
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd == -1) 
        return;

    int opt = 1;
    // allows the server to bind to an address that is in the TIME_WAIT state
    if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Setsockopt failed: " << strerror(errno) << std::endl;
        close(this->fd);
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(this->fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(this->fd);
        return;
    }

    if(listen(this->fd, 5) == -1)
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(this->fd);
        return;
    }
}

void Webserver::run()
{
    std::cout << "Server is running on http://localhost:" << this->port << std::endl;

    while(true)
    {
        Client client(this->fd);
        if (client.fd == -1)
            continue;
        
        std::string Response = get_response(client);

        send_response(client.fd, Response);

        close(client.fd);
    }
}

std::string Webserver::get_response(Client &client) 
{
    // request can targets to the CGI script
    if (client.request.find("GET /cgi-bin/hello.py") == 0)
        return cgi_execute("./src/cgi/hello.py");

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html><html><head><title>Hello, World!</title></head>"
        "<body><h1>Hello, World!</h1></body></html>";
    return response;
}

void Webserver::send_response(int client_fd, const std::string &response) 
{
    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}
