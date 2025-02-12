#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

const char header[] = 
"HTTP/1.1 200\r\n"
"Host: myawesomesite.org\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 158\r\n"
"\r\n";
const char site[] = 
"<!doctype html><html><head><title>My Site</title></head><body>"
"<center>"
"<h1>Hello world!</h1>"
"This is my awesome website written in C++!"
"</center></body></html>\r\n";

class Server {
private:
  const int _port;
  int _sockfd;
public:
  Server();
  ~Server();

  void run();
};

#endif