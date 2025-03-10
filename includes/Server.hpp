#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

// Should be read from a configuration file.
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define ROOT_DIR "./www"

class HttpRequest;

class Server {
    private:
        config_map *config;

        bool handle_client(int client_sock);
        void set_nonblocking(int sock);
        void listener(int server_sock);
        void send_response(int client_sock, const std::string &path);
    public:
        Server(config_map &config) : config(&config) {}
        ~Server() {}

        int start();
        void handleResponse(int client_sock, char *buffer);

        bool isDirectoryListing(const config_map *location, const FileData &fileData);

        const FileData createFileData(const config_map *location, HttpRequest &request) const;

        const config_map *findLocation(const std::string &path);
};


#endif