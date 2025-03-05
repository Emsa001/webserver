#include "Webserv.hpp"

int Server::start() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }
    
    // allow reusing the same port after the server has been stopped
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    const int port = this->config->at("listen");
    sockaddr_in server_addr;                        // → Structure to store the server’s IP address and port.
    memset(&server_addr, 0, sizeof(server_addr));   // → Clear the structure.
    server_addr.sin_family = AF_INET;               // → Set the address family to AF_INET (IPv4).
    server_addr.sin_addr.s_addr = INADDR_ANY;       // → Set the IP address to INADDR_ANY (all available interfaces).
    server_addr.sin_port = htons(port);             // → Set the port number.

    // std::cout << "Host order: " << std::hex << port << std::endl;
    // std::cout << "Network order: " << std::hex << server_addr.sin_port << std::endl;
    
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    this->listener(server_sock);

    close(server_sock);
    return 0;
}