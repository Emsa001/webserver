#include "HttpResponse.hpp"

void HttpResponse::build() {
    this->response = this->statusLine + "\r\n";
    for (StringMap::iterator it = headers.begin(); it != headers.end(); ++it) {
        this->response += it->first + ": " + it->second + "\r\n";
    }
    this->response += "\r\n" + body;
}