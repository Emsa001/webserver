#include "webserv.hpp"

int main()
{
    Config config;
    config.parse();

    std::cout << std::endl << std::endl << std::endl;

    std::cout << config.getRoot() << std::endl;
    std::cout << std::endl << std::endl << std::endl;

    config_array servers = config.getServers();
    std::cout << servers[1] << std::endl;

    std::cout << std::endl << std::endl << std::endl;

    return 0;
}

