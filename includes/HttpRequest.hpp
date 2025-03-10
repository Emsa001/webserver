#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "Webserv.hpp"

/*

https://www.rfc-editor.org/rfc/rfc2616#section-3.2.2

3.2.2 http URL

   The "http" scheme is used to locate network resources via the HTTP
   protocol. This section defines the scheme-specific syntax and
   semantics for http URLs.

   http_URL = "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]

   If the port is empty or not given, port 80 is assumed. The semantics
   are that the identified resource is located at the server listening
   for TCP connections on that port of that host, and the Request-URI
   for the resource is abs_path (section 5.1.2). The use of IP addresses
   in URLs SHOULD be avoided whenever possible (see RFC 1900 [24]). If
   the abs_path is not present in the URL, it MUST be given as "/" when
   used as a Request-URI for a resource (section 5.1.2). If a proxy
   receives a host name which is not a fully qualified domain name, it
   MAY add its domain to the host name it received. If a proxy receives
   a fully qualified domain name, the proxy MUST NOT change the host
   name.


*/

class HttpURL {

    private:      
        std::string path;
        std::string query;
        StringMap queryMap;

    public:
        HttpURL(const std::string &url);
        ~HttpURL(){}

        // Getters
        const std::string &getPath() const { return this->path; }
        const std::string &getQuery() const { return this->query; }
        const StringMap &getQueryMap() const { return this->queryMap; }
};

class HttpRequest {
    private:
        StringMap headers;

        std::string method;
        std::string uri;
        std::string version;  

        HttpURL *url;

        int socket; // Do we need this here?
    public:
        HttpRequest(int socket, char *buffer);
        ~HttpRequest() {
            delete url;
        };
        
        // Setters

        void setHeader(const std::string &key, const std::string &value) {  headers[key] = value; }
        void setMethod(const std::string &method) { this->method = method; }
        void setUri(const std::string &uri) { this->uri = uri; }
        void setVersion(const std::string &version) { this->version = version; }

        // Getters

        const std::string &getMethod() const { return this->method; }
        const std::string &getURI() const { return this->uri; }
        const std::string &getVersion() const { return this->version; }
        HttpURL *getURL() const { return this->url; }
        const StringMap &getHeaders() const { return this->headers; }
};




#endif