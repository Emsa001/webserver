#include "Webserv.hpp"
#include "SocketHandler.hpp"

SocketHandler::SocketHandler(config_array& servers) : _servers(servers) {
    std::cout << "SocketHandler()" << std::endl;
}

SocketHandler::SocketHandler() {
    Config& config = Config::instance();
    config.parse("conf/default.yml");
    _servers = config.getServers();
}

SocketHandler::~SocketHandler() {
    std::cout << "~SocketHandler" << std::endl;
}

int SocketHandler::run() {
    struct addrinfo hints, *res;
    int r;
    if(r = getaddrinfo("127.0.0.1", "8080", NULL, &res) < 0)
        perror("getaddrinfo");
    printf("%p\n", res);
    int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (errno != 0) {
        // perror("socket");
        exit(EXIT_FAILURE);
    }
}