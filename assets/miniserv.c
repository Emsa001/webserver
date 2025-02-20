#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char header[] = 
"HTTP/1.1 200\r\n"
"Host: myawesomesite.org\r\n"
"Content-Type: text/html\r\n"
"Content-Length: 156\r\n"
"\r\n";
const char site[] = 
"<!doctype html><html><head><title>My Site</title></head><body>"
"<center>"
"<h1>Hello world!</h1>"
"This is my awesome website written in C!"
"</center></body></html>\r\n";

void errexit(const char *err) {
  fputs("Error: ", stderr);
  fputs(err, stderr);
  fputs("\n", stderr);
  exit(1);
}

int main() {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    errexit("can't create socket");
  struct sockaddr_in a;
  memset(&a, 0, sizeof(a));
  a.sin_family = AF_INET;
  a.sin_port = htons(80);
  if (bind(sock, (struct sockaddr *) &a, sizeof(a)) < 0)
    errexit("can't bind\n");
  if (listen(sock, 1) < 0)
    errexit("can't listen\n");

  struct sockaddr_in peeraddr;
  socklen_t peeraddr_len = sizeof(peeraddr);

  while (1) {
    int fd = accept(sock, (struct sockaddr *) &peeraddr, &peeraddr_len);
    if (fd < 0)
      errexit("accept() failed");
    printf("GET\n");
    send(fd, (void *) header, strlen(header), 0);
    send(fd, (void *) site, strlen(site), 0);
    close(fd);
  }
  close(sock);
}
