#include "HttpResponse.hpp"

void HttpResponse::respond() {
    this->setHeader("Server", PROJECT_NAME);
    // this->setHeader("Connection", "keep-alive");
    // this->setHeader("Keep-Alive", "timeout=10, max=100");

    this->build();

    size_t total_sent = 0;
    while (total_sent < this->response.size()) {
        size_t sent = send(this->socket, this->response.c_str() + total_sent,
                            this->response.size() - total_sent, 0);
        if (sent <= 0) {
            std::cerr << "Error sending response\n";
            break;
        }
        total_sent += sent;
    }

    std::cout << "Response sent immediately for socket: " << this->socket << std::endl;
}
