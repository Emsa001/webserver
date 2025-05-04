#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"

class HttpRequest;
struct ClientRequestState;

class Server {
    private:
        config_map *config;
        int keep_alive;

        bool handleResponse(int client_sock, HttpRequest *request);

        void isValidMethod(HttpRequest *request, const config_map &location);
        const FileData createFileData(const config_map *location, HttpRequest *request) const;

    public:
        Server(config_map &config) : config(&config) {
            this->keep_alive = Config::getSafe(config, "keep_alive", 30).getInt();;
        }
        ~Server() {}

        int start();

        bool isDirectoryListing(const config_map *location, const FileData &fileData);
        const config_map *findLocation(const std::string &path);
        void closeConnection(int *client_sock);

        std::string const getServerName() const {
            return this->config->at("server_name");
        }

};


#endif
