#include "Webserv.hpp"

void signalHandler(int signum) {
    pthread_mutex_lock(&g_stop_mutex);
    g_stop = 1;
    pthread_mutex_unlock(&g_stop_mutex);

    std::cout << "Signal (" << signum << ") received." << std::endl;
    std::cout << "Stopping all servers..." << std::endl << std::endl;
}
