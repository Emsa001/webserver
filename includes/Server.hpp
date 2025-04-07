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

        std::vector<pollfd> fds;
        std::map<int, time_t> client_timestamps;

        void listener(int server_sock);
        void setNonBlocking(int sock);
        void acceptNewConnections(int server_sock);
        void handleClientRead(size_t index);
        void checkIdleClients();
        void removeClient(size_t index);
        bool handleResponse(int client_sock, const char *buffer);

        void logs(HttpRequest &request);

    public:
        Server(config_map &config) : config(&config) {}
        ~Server() {}

        int start();
        bool isDirectoryListing(const config_map *location, const FileData &fileData);
        const FileData createFileData(const config_map *location, HttpRequest &request) const;
        const config_map *findLocation(const std::string &path);
        void closeConnection(int *client_sock);
};


#endif