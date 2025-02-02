#include "../includes/webserv.hpp"

int main() 
{
    Server server(PORT);
    handle_signal();
    server.run();

    return 0;
}
