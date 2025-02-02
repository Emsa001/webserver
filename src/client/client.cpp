#include "../../includes/webserv.hpp"

Client::Client(int server_fd) 
{
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    this->fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
    if (this->fd == -1)
        return;

    char buffer[1024] = {0};
    read(fd, buffer, sizeof(buffer) - 1);
    
    this->request = buffer;
}
