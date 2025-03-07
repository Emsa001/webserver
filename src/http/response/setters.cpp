#include "HttpResponse.hpp"

void HttpResponse::setBody(const std::string &path){
    std::string content = readFile(path);

    if (fileExists(path) == false) {
        this->body = "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>";
        this->setStatusCode(404);
        this->setHeader("Content-Type", "text/html");
        this->setHeader("Content-Length", intToString(this->body.size()));
        return ;
    }

    this->body = content;
    this->setStatusCode(200);
    this->setHeader("Content-Type", HttpResponse::getMimeType(path));
    this->setHeader("Content-Length", intToString(this->body.size()));

}
