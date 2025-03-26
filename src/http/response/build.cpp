#include "HttpResponse.hpp"

void HttpResponse::build() {

    this->setHeader("Server", PROJECT_NAME);
    this->setHeader("Connection", "keep-alive");
    this->setHeader("Keep-Alive", "timeout=10, max=100");

    this->response = this->statusLine + "\r\n";
    for (StringMap::iterator it = headers.begin(); it != headers.end(); ++it) {
        this->response += it->first + ": " + it->second + "\r\n";
    }
    this->response += "\r\n" + body;
}