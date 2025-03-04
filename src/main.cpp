#include "Webserv.hpp"

int main()
{
    std::cout << std::endl << std::endl << std::endl;

    Config config("conf/myserver.yml");
    if(!config.parse()){
        return 1;
    }

    // std::cout << std::endl << std::endl << std::endl;

    // std::cout << config.getRoot() << std::endl;

    // std::string log_format = config.getRoot()["log_format"];

    // std::cout << std::endl << log_format << std::endl << std::endl;

    // config_array servers = config.getServers();
    // std::cout << servers[0] << std::endl;

    // std::cout << std::endl << std::endl << std::endl;


    config_array servers = config.getServers();
    config_array::iterator it = servers.begin();

    for(;it != servers.end(); it++){
        std::string server_name = it->getMap()["server_name"];
        std::cout << "Server name: " << server_name << std::endl;

        Server server(it->getMap());
        server.start();
    }

    return 0;
}

