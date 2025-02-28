#include "Webserv.hpp"

int main()
{
    std::cout << std::endl << std::endl << std::endl;

    Config config("conf/myserver.yml");
    if(!config.parse()){
        return 1;
    }

    std::cout << std::endl << std::endl << std::endl;

    // std::cout << config.getRoot() << std::endl;


    // std::string log_format = config.getRoot()["log_format"];

    // std::cout << std::endl << log_format << std::endl << std::endl;

    config_array servers = config.getServers();
    std::cout << servers[0] << std::endl;


    // std::cout << std::endl << std::endl << std::endl;

    return 0;
}

