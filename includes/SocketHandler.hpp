#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <Server.hpp>
#include <map>
#include <vector>
#include <poll.h>

using namespace std;

/* Class in charge of controlling and poll()-ing the
server sockets.

It will listen on sockets and direct incoming requests
to the correct Server instance to be processed.

For this it accepts a config_array servers in its constructor*/
class SocketHandler {
// private:
//   void handleConnections();
protected:

  // std::vector<pollfd> fds;
  // std::map<int, time_t> client_timestamps;
  // std::map<int, ClientRequestState> requestStates;

  // void listener(int server_sock);
  // void setNonBlocking(int sock);
  // void acceptNewConnections(int server_sock);
  // void handleClientRead(size_t index);
  // void checkIdleClients();
  // void removeClient(size_t index);
  // ClientRequestState *readChunk(int fd, int index, char *buffer);
  // int readBytes(int fd, int index, char *buffer);
  
  const config_array& _servers;
  
  /* contiguous store of `pollfd` structs
  this pointer can thus be directly passed to `poll()` */
  vector<pollfd> _pollfds;
  /* map from fd to pending request
  keeps buffer and state of incoming requests */
  map<int, ClientRequestState> _pendingRequests;

  /* Initialize socket for server*/
  int socketInit(const config_map &server);
public:
  SocketHandler(const config_array& servers);
  ~SocketHandler();
  int run();
};

#endif