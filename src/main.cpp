#include "Webserv.hpp"

volatile sig_atomic_t g_stop = 0;
pthread_mutex_t Logger::logMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_stop_mutex = PTHREAD_MUTEX_INITIALIZER;

void* startServer(void* arg) {
    config_map* data = static_cast<config_map*>(arg);
    Server server(*data);
    std::cout << std::endl;
    server.start();
    std::cout << std::endl;
    return NULL;
}

void signalHandler(int signum) {
    pthread_mutex_lock(&g_stop_mutex);
    g_stop = 1;
    pthread_mutex_unlock(&g_stop_mutex);

    std::cout << "Signal (" << signum << ") received." << std::endl;
    std::cout << "Stopping all servers..." << std::endl << std::endl;
}

int main()
{
    signal(SIGINT, signalHandler);

    std::cout << std::endl << std::endl << std::endl;

    Config& config = Config::instance();
    config.parse("conf/cgi.yml");

    config_array servers = config.getServers();
    config_array::iterator it = servers.begin();

    std::vector<pthread_t> threads;

    for(; it != servers.end(); it++){
        pthread_t thread;
        pthread_create(&thread, NULL, startServer, &it->getMap());
        threads.push_back(thread);
    }

    for(size_t i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], NULL);
    }

    Logger::destroy();

    return 0;
}