#include "Client.hpp"

#define MAX_REQUEST_SIZE 8192 

Client::Client(int client_fd)
{
    this->fd = client_fd;
    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        this->request.append(buffer, bytes_read);

        // 🚨 Prevent infinite request growth
        if (this->request.size() > MAX_REQUEST_SIZE) {
            std::cerr << "Error: Request too large, closing connection.\n";
            break;
        }

        size_t contentLengthPos = this->request.find("Content-Length: ");

        if (contentLengthPos == std::string::npos) {
            if (this->request.find("\r\n\r\n") != std::string::npos) {
                break;
            }
            continue; 
        }

        size_t start = contentLengthPos + 16;
        size_t end = this->request.find("\r\n", start);
        if (end == std::string::npos) {
            continue;  // Wait for full Content-Length header
        }

        int contentLength = atoi(this->request.substr(start, end - start).c_str());
        size_t bodyStart = this->request.find("\r\n\r\n");

        if (bodyStart != std::string::npos && this->request.size() >= bodyStart + 4 + contentLength) {
            break;  // Full request is received
        }
    }

    if (bytes_read == 0) {
        std::cout << "Client closed the connection.\n";
    } else if (bytes_read == -1) {
        perror("read");
    }
}
