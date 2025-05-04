#include "Webserv.hpp"

int Server::readBytes(int fd, int index, char *buffer){
    const int bytes_read = recv(fd, buffer, sizeof(buffer), 0);

    if (bytes_read < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
        Logger::error("Recv error on fd " + intToString(fd) + ", closing connection.");
        this->removeClient(index);
    }

    if(bytes_read == 0){
        this->removeClient(index);
    }

    return bytes_read;
}

// This is a bit ugly, but Im too lazy to refactor it
ClientRequestState *Server::readChunk(int fd, int index, char *buffer) {
    int bytes_read = this->readBytes(fd, index, buffer);
    if (bytes_read <= 0) return NULL;

    ClientRequestState &state = requestStates[fd];
    state.buffer.append(buffer, bytes_read);

    // Optional: prevent abuse with a hard limit for a chunk
    if (state.buffer.size() > HARD_BUFFER_LIMIT) {
        Logger::warning("Request exceeded hard buffer limit on fd " + intToString(fd));
        this->removeClient(index);
        return NULL;
    }

    size_t headerEnd = state.buffer.find("\r\n\r\n");
    if (headerEnd != std::string::npos && !state.headersParsed) {
        state.headersParsed = true;

        // Soft parse just Content-Length to know when to stop reading
        std::string headers = state.buffer.substr(0, headerEnd);
        size_t clPos = headers.find("Content-Length:");

        if (clPos != std::string::npos) {
            size_t start = headers.find_first_of("0123456789", clPos);
            if (start != std::string::npos) {
                state.contentLength = stringToInt(headers.substr(start));
            }
        } else {
            state.contentLength = 0;
        }

        // Compute total required bytes: headers + body
        state.expectedSize = headerEnd + 4 + state.contentLength;
    }

    // All data we need is available
    if (state.headersParsed && state.buffer.size() >= state.expectedSize) {
        return &state;
    }

    // Still waiting for more data
    return NULL; 
}


void Server::handleClientRead(size_t index) {
    int fd = this->fds[index].fd;
    char buffer[READ_BUFFER_SIZE];
    
    ClientRequestState *state = this->readChunk(fd, index, buffer);
    if(state == NULL) return;

    if (state->headersParsed) {
        size_t totalRequired = state->buffer.find("\r\n\r\n") + 4 + state->contentLength;

        if (state->buffer.size() >= totalRequired) {
            HttpRequest request(fd, state->buffer);

            request.setMaxHeaderSize(Config::getSafe(*this->config, "max_client_header_size", DEFAULT_MAX_HEADER_SIZE));
            request.setMaxBodySize(Config::getSafe(*this->config, "max_client_body_size", DEFAULT_MAX_BODY_SIZE));
            
            if (!this->handleResponse(fd, &request)) {
                this->removeClient(index);
            }

            requestStates.erase(fd);
        }
    }
}
