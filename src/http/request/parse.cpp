#include "HttpRequest.hpp"

void HttpRequest::parse() {
    std::string request(this->buffer);
    std::string line;
    std::istringstream requestStream(request);

    
    std::getline(requestStream, line);
    
    size_t totalHeaderSize = line.size();
    if (totalHeaderSize > this->maxHeaderSize) {
        throw HttpRequestException(414);
    }

    std::istringstream lineStream(line);
    lineStream >> this->method >> this->uri >> this->version;

    // Parse the URL
    this->url = new HttpURL(this->uri);

    // Parse the headers
    while (std::getline(requestStream, line) && line != "\r") {
        if (line.empty() || line == "\r\n") break;

        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);

            key.erase(0, key.find_first_not_of(" \t\n\r"));
            key.erase(key.find_last_not_of(" \t\n\r") + 1);
            value.erase(0, value.find_first_not_of(" \t\n\r"));
            value.erase(value.find_last_not_of(" \t\n\r") + 1);

            totalHeaderSize += line.size();
            this->headers[key] = value;
        }

        if (totalHeaderSize > this->maxHeaderSize) {
            throw HttpRequestException(431);
        }
    }

    // Parse the body
    if (this->method == "POST" || this->method == "DELETE") {
        std::ostringstream bodyStream;

        bodyStream << requestStream.rdbuf();
        this->body = bodyStream.str();
        if (this->body.size() > this->maxBodySize) {
            throw HttpRequestException(413);
        }
    }
}