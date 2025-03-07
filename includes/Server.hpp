#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"
#include "Client.hpp"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define ROOT_DIR "./www"

extern bool SERVER_STOP;
class Client;

class Server {
    private:
        Server() {};
        config_map *config;
        int fd;
        
        std::string get_response(Client &client);
        void send_response(int client_fd, const std::string &response);
    public:
        Server(config_map &config);
        ~Server();
        void run();
        void set_nonblocking(int sock);
        std::string GET(const std::string &request);
        std::string POST(const std::string &request);
        std::string DELETE(const std::string &request);
        
};

std::string get_mime_type(const std::string &path);

#endif