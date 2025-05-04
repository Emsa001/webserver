#include <Webserv.hpp>
#include <SocketHandler.hpp>


#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
//#include <netinet/ip.h>
//#include <arpa/inet.h>
#include <poll.h>

#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <istream>

// using namespace SocketHandler;

// void SocketHandler::handleConnections() {
    
// }

using namespace std;

// todo: host
void make_addr(struct sockaddr_in *addr, int port) {
  memset(addr, 0, sizeof(struct sockaddr_in));
  addr->sin_family = AF_INET;
  addr->sin_port = htons(port);
}

SocketHandler::SocketHandler(const config_array& servers) : _servers(servers), _pollfds(_servers.size()) {
  socketInit(servers[0]);
  // _socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, proto_num);
  // std::cout << "socket: " << _socket << std::endl;
  // // build AF_INET jddress with 0.0.0.0:80
  // struct sockaddr_in addr;
  // memset(&addr, 0, sizeof(addr));
  // addr.sin_family = AF_INET;
  // addr.sin_port = htons(8000);
  //addr.in_addr = 0;
}

SocketHandler::~SocketHandler() {
  // close(_socket);
}

int SocketHandler::run() {
  while (true) {
    // struct pollfd pfds[1];
    // pfds[0].fd = _socket;
    // pfds[0].events = POLLIN; // | POLLOUT | POLLERR | POLLHUP;
    // pfds[0].revents = 0;

    std::cout << "polling" << _pollfds[0].revents << ", " << _pollfds[0].fd << ::endl;
    _pollfds[0].revents = 0;
    int p = poll((struct pollfd *) &_pollfds[0], 1, 1000);
    if (p == 0) {
      std::cout << "nada" << std::endl;
      continue;
    }
    if (_pollfds[0].revents & POLLIN) {
      std::cout << "accepting connection" << std::endl;
      char buffer[1000];
      _pollfds[0].fd = accept(_pollfds[0].fd, NULL, NULL);
      int bytes_read = read(_pollfds[0].fd, buffer, 999);
      if (bytes_read < 0)
          cout << "error: res" << std::endl;
      buffer[bytes_read] = '\0';
      std::cout << "got data" << std::endl;
      write(_pollfds[0].fd, "test\n", 5);
      std::cout << buffer << std::endl;
      close(_pollfds[0].fd);
    }
    if (_pollfds[0].revents & (POLLERR | POLLHUP)) {
      std::cout << "error" << std::endl;
      break;
    }
  }
  return 0;
}

int SocketHandler::socketInit(const config_map &server) {
  // select protocol
  int proto_num = getprotobyname("tcp")->p_proto;
  struct pollfd pfd;
  pfd.fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, proto_num);
  std::cout << "socket fd is " << pfd.fd << std::endl;
  pfd.events = POLLIN | POLLERR | POLLHUP;
  std::cout << "events is " << pfd.events << std::endl;
  pfd.revents = 0;
  if (pfd.fd <= 0) {
    perror("error: ");
  }
  
  // bind & listen
  struct sockaddr_in addr;
  make_addr(&addr, 8000);
  if (bind(pfd.fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    perror("error: ");
  }
  if (listen(pfd.fd, 100) == -1) { //todo: max connections
    perror("error: ");
  }

  _pollfds.push_back(pfd);

  return 0;
}