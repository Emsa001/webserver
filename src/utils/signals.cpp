#include "../../includes/webserv.hpp"

bool SERVER_STOP = false;

void change_stop(int signal) 
{
    if (signal == SIGINT) 
        SERVER_STOP = true;
}

void handle_signal() 
{
    struct sigaction sa;
    sa.sa_handler = change_stop;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
}
