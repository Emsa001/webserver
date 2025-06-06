#include "Webserv.hpp"

// volatile sig_atomic_t g_stop = 0;

void* startServer(void* arg) {
    config_map* data = static_cast<config_map*>(arg);
    Server server(*data);
    std::cout << std::endl;
    server.start();
    std::cout << std::endl;
    return NULL;
}

int main()
{
    signal(SIGINT, signalHandler);
    std::cout << std::endl << std::endl << std::endl;

    Logger::init();

    // Config& config = Config::instance();
    // config.parse("conf/default.yml");

    //TODO: replace threads

    // Server s(it->getMap());
    // s.start();
    // TODO: initialize servers

    Logger::destroy();

    return 0;
}
