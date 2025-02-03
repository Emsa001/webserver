#include "../../includes/client.hpp"

Client::Client(int client_fd)
{
    this->fd = client_fd;

    char buffer[1024] = {0};
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    
    if (bytes_read > 0)
        this->request = buffer;
}