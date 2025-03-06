#include "HttpResponse.hpp"

void HttpResponse::setBody(const std::string &path){
    std::string content = read_file(path);
    if (content.empty()) {
        this->body = "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>";
        this->setStatusCode(404);
        this->setHeader("Content-Type", "text/html");
        return ;
    }

    this->body = content;
    this->setStatusCode(200);
    this->setHeader("Content-Type", HttpResponse::getMimeType(path));

}
