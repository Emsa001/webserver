#include "webserv.hpp"

int main()
{
    // handle_signal();

    Config config;
    config.parse();

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    ConfigValue logFormat = config.getRoot()["log_format"];
    config_map root = config.getRoot();
    config_array servers = config.getServers();

    std::cout << "log_format: " << logFormat << std::endl;

    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << servers[i] << std::endl;
    }

    return 0;
}

