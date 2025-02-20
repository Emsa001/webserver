#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "webserv.hpp"

class Client
{
    private:
        int fd;
        std::string request;
    public:

        Client(int server_fd);
        ~Client() { close(this->fd); }

        std::string get_request() { return this->request; }
        int get_fd() { return this->fd; }
};



#endif