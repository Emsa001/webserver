#include "../includes/webserv.hpp"

bool stop = false;

int fd = -1;

void handle_signal(int signal) 
{
    if (signal == SIGINT) 
    {
        stop = true;
        close(fd);
    }
}

int main() 
{
    Webserver server(PORT);

    fd = server.get_fd();
    signal(SIGINT, handle_signal);
    
    server.run();

    return 0;
}
