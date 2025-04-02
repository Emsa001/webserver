#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "Webserv.hpp"

class HttpResponse {
    private:
        std::string statusLine;  // Stores "HTTP/1.1 200 OK"
        StringMap headers;      // Stores key-value pairs like "Content-Type: text/html"
        std::string body;        // Stores the response body (usually html)

        bool listing;           // If true, the body will be a directory listing
        bool cgi;               // If true, the body will be the output of a CGI script

        std::string response;    // Stores the entire response to be sent to the client

        unsigned short statusCode;
        int socket;
    
    public:
        HttpResponse(int socket) : socket(socket) {
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
        
        std::string getReasonPhrase(unsigned short code);
        static std::string getMimeType(const std::string &path);
        
        void directoryListing(const FileData &fileData);
        void respondStatusPage(unsigned short code);

        void setBody(FileData &fileData);
        void setStatusCode(unsigned short code) {
            this->statusCode = code;
            statusLine = "HTTP/1.1 " + intToString(code) + " " + this->getReasonPhrase(code);
        }

        void setResponse(const std::string &response) { this->response = response; }
        void setHeader(const std::string &key, const std::string &value) { headers[key] = value; }
        void setListing(bool listing) { this->listing = listing; }
        void setCGI(bool cgi) { this->cgi = cgi; }

        StringMap getHeaders() const { return headers; }
};

#endif