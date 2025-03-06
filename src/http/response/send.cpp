#include "HttpResponse.hpp"

void HttpResponse::respond(){

    this->setHeader("Connection", "keep-alive");
    this->buildResponse();

    send(this->socket, this->response.c_str(), this->response.size(), 0);
}
