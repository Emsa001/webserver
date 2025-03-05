#include "HttpResponse.hpp"

void HttpResponse::respond(const std::string &path){

    this->setHeader("Content-Type", this->getMimeType(full_path));
    this->setHeader("Content-Length", intToString(content.size()));
    this->setHeader("Connection", "keep-alive");
    this->setBody(content);

    this->buildResponse();

    send(this->client_socket, this->response.c_str(), this->response.size(), 0);
}
