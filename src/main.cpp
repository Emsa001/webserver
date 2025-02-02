#include "../includes/webserv.hpp"

int server_fd = -1;

void handle_signal(int signal) 
{
    if (signal == SIGINT) 
    {
        std::cout << "\nServer is shutting down." << std::endl;
        if (server_fd != -1) 
        {
            close(server_fd);
            std::cout << "Server socket closed." << std::endl;
        }
        exit(0); // we cant use exit :/
    }
}

int main() 
{
    Webserver server(PORT);

    server_fd = server.get_fd();

    signal(SIGINT, handle_signal);
    
    server.run();

    return 0;
}
