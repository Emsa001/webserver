#ifndef SERVER_HPP
#define SERVER_HPP
#include "webserv.hpp"

class Client;

class Server
{
    private:
        int fd;
        int port;
    public:
        Server(int port);
        ~Server();

        void run();
        std::string get_response(Client &client);
        void send_response(int client_fd, const std::string &response);
};

#endif