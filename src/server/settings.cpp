#include "Server.hpp"

/* 
    Function to set a socket as non-blocking. This is useful for handling multiple clients.
    
    - In blocking mode, accept(), recv(), and send() will stop the program if no data is available. 
    
    - In non-blocking mode, the program continues running even if there’s no incoming connection or data.
*/

void Server::set_nonblocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}