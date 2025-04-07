#include "HttpResponse.hpp"

void HttpResponse::respond() {
    size_t total_sent = 0;
    const char *response_ptr = this->response.c_str();
    size_t response_size = this->response.size();

    while (total_sent < response_size) {
        ssize_t sent = send(this->socket, response_ptr + total_sent, response_size - total_sent, MSG_NOSIGNAL);
        
        if (sent < 0) {
            std::cerr << "Error sending response on socket: " << this->socket << " (" << strerror(errno) << ")" << std::endl;
            return;
        }

        total_sent += sent;
    }

    std::cout << "Finished sending response on socket: " << this->socket << std::endl;
}
