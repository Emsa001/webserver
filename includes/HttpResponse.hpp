#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "Webserv.hpp"

class HttpRequest;

class HttpResponse {
    private:

        HttpRequest *request; // Pointer to the request object

        std::string statusLine;  // Stores "HTTP/1.1 200 OK"
        StringMap headers;      // Stores key-value pairs like "Content-Type: text/html"
        std::string body;        // Stores the response body (usually html)

        bool listing;           // If true, the body will be a directory listing
        bool cgi;               // If true, the body will be the output of a CGI script

        std::string response;    // Stores the entire response to be sent to the client

        unsigned short statusCode;
        int socket;
    
    public:
        HttpResponse(int socket, HttpRequest *request) : request(request), socket(socket) {
            statusLine = "HTTP/1.1 200 OK";
            listing = false;
            statusCode = 200;
            body = "";
            cgi = false;
        };
        ~HttpResponse() {};


        // Methods
        void respond();
        void build();
        void log();
        
        std::string getReasonPhrase(unsigned short code);
        static std::string getMimeType(const std::string &path);
        
        void directoryListing(const FileData &fileData);
        void respondStatusPage(unsigned short code);

        void buildBody(FileData &fileData);
        void setBody(std::string &body) { this->body = body; }
        void setStatusCode(unsigned short code) {
            this->statusCode = code;
            statusLine = "HTTP/1.1 " + intToString(code) + " " + this->getReasonPhrase(code);
        }

        // Setters
 
        void setResponse(const std::string &response) { this->response = response; }
        void setHeader(const std::string &key, const std::string &value) { headers[key] = value; }
        void setSettings(const config_map *location);

        // Getters

        StringMap getHeaders() const { return headers; }
};

#endif