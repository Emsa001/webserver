#ifndef SERVER_HPP
#define SERVER_HPP

#include "Config.hpp"

#include <iostream> // → Allows us to use std::cout and std::cerr for logging.
#include <fstream> // → Used to read files (to serve static files).
#include <sstream> // → Used for string manipulation (converting HTTP responses to strings).
#include <string> // → Allows us to use std::string for text handling.
#include <vector> // → Used for storing multiple sockets in poll().
#include <cstring> // → Used for memset() to clear data structures.
#include <sys/types.h> // → Required for networking functions.
#include <sys/socket.h> // → Provides functions like socket(), bind(), listen(), accept(), etc.
#include <netinet/in.h> // → Contains sockaddr_in (for IP addresses and ports).
#include <unistd.h> // → Provides close() function for sockets.
#include <fcntl.h> // → Used to set sockets as non-blocking.
#include <poll.h> // → Allows us to use poll() for monitoring multiple clients.

// Should be read from a configuration file.
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define ROOT_DIR "./www"

class Server {
    private:
        config_map *config;

        void handle_client(int client_sock);
        void set_nonblocking(int sock);
        void listener(int server_sock);
        void send_response(int client_sock, const std::string &path);
    public:
        Server(config_map &config) : config(&config) {}
        ~Server() {}

        int start();
};

std::string get_mime_type(const std::string &path);

#endif