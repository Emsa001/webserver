#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#define PORT 8080

std::string cgi_execute(const std::string &scriptPath);

class Client
{
    private:
    public:
        int fd;
        std::string request;

        Client(int server_fd);
        ~Client() { close(this->fd); }
};

class Webserver
{
    private:
        int fd;
        int port;
    public:
        Webserver(int port);
        ~Webserver() { 
            close(this->fd); 
            std::cout << "Server socket closedddddddddddd." << std::endl;
            }

        void run();
        int get_fd() { return this->fd; }
        std::string get_response(Client &client);
        void send_response(int client_fd, const std::string &response);
};

#endif
