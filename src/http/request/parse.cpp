#include "HttpRequest.hpp"

void HttpRequest::parse() {
    size_t headerEnd = rawRequestData.find("\r\n\r\n");
    if (headerEnd == std::string::npos)
        throw HttpRequestException(400);

    std::string headerPart = rawRequestData.substr(0, headerEnd);
    std::string bodyPart = rawRequestData.substr(headerEnd + 4);

    std::istringstream headerStream(headerPart);
    std::string line;

    std::getline(headerStream, line);
    size_t totalHeaderSize = line.size();
    if (totalHeaderSize > this->maxHeaderSize) {
        throw HttpRequestException(414);
    }

    std::istringstream lineStream(line);
    lineStream >> this->method >> this->uri >> this->version;
    this->url = new HttpURL(this->uri);

    while (std::getline(headerStream, line) && line != "\r") {
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

    if (this->method == "POST" || this->method == "DELETE") {
        this->body = bodyPart;
        if (this->body.size() > this->maxBodySize) {
            throw HttpRequestException(413);
        }
    }
}
