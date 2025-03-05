#include "HttpResponse.hpp"

void HttpResponse::buildResponse() {
    this->setHeader("Server", PROJECT_NAME);

    this->response = this->statusLine + "\r\n";
    for (strstr_map::iterator it = headers.begin(); it != headers.end(); ++it) {
        this->response += it->first + ": " + it->second + "\r\n";
    }
    this->response += "\r\n" + body;
}