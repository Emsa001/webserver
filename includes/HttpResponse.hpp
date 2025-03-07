#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "Webserv.hpp"

class HttpResponse {
    private:
        std::string statusLine;  // Stores "HTTP/1.1 200 OK"
        StringMap headers;      // Stores key-value pairs like "Content-Type: text/html"
        std::string body;        // Stores the response body (usually html)

        std::string response;    // Stores the entire response to be sent to the client

        unsigned short statusCode;
        int socket;
    
    public:
        HttpResponse(int socket) : socket(socket) {};
        ~HttpResponse() {};


        // Methods
        void respond();
        void build();
        
        std::string getReasonPhrase(unsigned short code);
        static std::string getMimeType(const std::string &path);
        
        // Setters

        void setHeader(const std::string &key, const std::string &value) {
            headers[key] = value;
        }

        void setBody(const std::string &body);

        void setStatusCode(unsigned short code) {
            this->statusCode = code;
            statusLine = "HTTP/1.1 " + intToString(code) + " " + this->getReasonPhrase(code);
        }

};

#endif