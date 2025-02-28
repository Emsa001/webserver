#include "Webserv.hpp"

int main()
{
    Config config("conf/myserver.yml");
    if(!config.parse())
        return 1;

    std::cout << std::endl << std::endl << std::endl;
    std::cout << config.getRoot() << std::endl;
    std::cout << std::endl << std::endl << std::endl;
    
    Server server(PORT);
    server.run();

    return 0;
}

