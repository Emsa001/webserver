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

std::string get_mime_type(const std::string &path) 
{
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".css") != std::string::npos) return "text/css";
    if (path.find(".js") != std::string::npos) return "application/javascript";
    if (path.find(".png") != std::string::npos) return "image/png";
    if (path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos) return "image/jpeg";
    return "text/plain";
}
