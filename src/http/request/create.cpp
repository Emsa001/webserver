#include "HttpRequest.hpp"

HttpRequest::HttpRequest(int socket, const char *buffer) {
    
    this->socket = socket;
    std::string request(buffer);
    std::string line;
    std::istringstream requestStream(request);

    // Parse the first line
    std::getline(requestStream, line);
    std::istringstream lineStream(line);
    lineStream >> this->method >> this->uri >> this->version;

    // Parse the headers
    while (std::getline(requestStream, line) && !line.empty()) {
        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            this->headers[key] = value;
        }
    }

    // Parse the URL
    this->url = new HttpURL(this->uri);
}