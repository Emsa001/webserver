#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "Webserv.hpp"

class HttpResponse {
    private:
        std::string statusLine;  // Stores "HTTP/1.1 200 OK"
        strstr_map headers;      // Stores key-value pairs like "Content-Type: text/html"
        std::string body;        // Stores the response body (usually html)

        std::string response;    // Stores the entire response to be sent to the client

        unsigned short statusCode;
        int client_socket;
    
    public:
        HttpResponse(int client_socket) : client_socket(client_socket) {};
        ~HttpResponse() {};


        // Methods
        void respond(const std::string &path);
        void buildResponse();
        
        std::string getReasonPhrase(unsigned short code);
        std::string getMimeType(const std::string &path);
        
        // Setters

        void setHeader(const std::string &key, const std::string &value) {
            headers[key] = value;
        }

        void setBody(const std::string &body) {
            this->body = body;
        }

        void setStatusCode(unsigned short code) {
            this->statusCode = code;
            statusLine = "HTTP/1.1 " + intToString(code) + " " + this->getReasonPhrase(code);
        }

};

#endif