#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"

class HttpRequest;
struct ClientRequestState;

class Server {
    private:
       
        config_map *config;
        int keep_alive;

        std::vector<pollfd> fds;
        std::map<int, time_t> client_timestamps;
        std::map<int, ClientRequestState> requestStates;

        void listener(int server_sock);
        void setNonBlocking(int sock);
        void acceptNewConnections(int server_sock);
        void handleClientRead(size_t index);
        void checkIdleClients();
        void removeClient(size_t index);

        bool handleResponse(int client_sock, HttpRequest *request);

        void isValidMethod(HttpRequest *request, const config_map &location);
        const FileData createFileData(const config_map *location, HttpRequest *request) const;

        ClientRequestState *readChunk(int fd, int index, char *buffer);
        int readBytes(int fd, int index, char *buffer);
    public:
        Server(config_map &config) : config(&config) {
            this->keep_alive = Config::getSafe(config, "keep_alive", 30).getInt();;
        }
        ~Server() {}

        int start();
        bool isStop();

        bool isDirectoryListing(const config_map *location, const FileData &fileData);
        const config_map *findLocation(const std::string &path);
        void closeConnection(int *client_sock);

        std::string const getServerName() const {
            return this->config->at("server_name");
        }

};


#endif
