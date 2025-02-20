#include "server.hpp"

Server::Server(int port)
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

Server::~Server()
{
    close(this->fd);
    std::cout << "\nDestructor called" << std::endl;
}

void Server::simple_run()
{
    std::cout << "Server is running on http://localhost:" << this->port << std::endl;

    while(1)
    {
        Client client(this->fd);

        int fd = client.get_fd();

        if (fd == -1)
            continue;
        
        std::string Response = get_response(client);
        std::cout << "Client joined" << std::endl;

        send_response(fd, Response);
        close(fd);
    }
}

void Server::run()
{
    
    std::string request = client.get_request();

    if (request.find("GET /cgi-bin/hello.py") == 0)
        return cgi_execute("./src/cgi/hello.py");

    struct pollfd fds[200];
    bzero(fds, sizeof(fds));
    int nfds = 1;

    fds[0].fd = this->fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    
    while (!SERVER_STOP)
    {
        int poll_count = poll(fds, nfds, -1);
        if (poll_count == -1)
        {
            std::cerr << "Poll failed: " << strerror(errno) << std::endl;
            return;
        }

        for (int i = 0; i < nfds; i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == this->fd)
                {
                    sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(this->fd, (sockaddr *)&client_addr, &client_len);
                    if (client_fd == -1)
                    {
                        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
                        continue;
                    }

                    fds[nfds].fd = client_fd;
                    fds[nfds].events = POLLIN;
                    nfds++;
                }
                else
                {
                    Client client(fds[i].fd);
                    std::string response = get_response(client);
                    send_response(fds[i].fd, response);

                    for (int j = i; j < nfds - 1; j++)
                        fds[j] = fds[j + 1];
                    nfds--;
                }
            }
        }
    }
}

void Server::send_response(int client_fd, const std::string &response) 
{
    send(client_fd, response.c_str(), response.size(), 0);
    close(client_fd);
}
