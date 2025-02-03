#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "webserv.hpp"

class Client
{
    private:
    public:
        int fd;
        std::string request;

        Client(int server_fd);
        ~Client() { close(this->fd); }
};



#endif