#include "webserv.hpp"

int main() 
{
    handle_signal();

    Config config;
    config_servers servers = config.getServers();

    int port = servers[0]["listen"];

    Server server(port);
    server.run();


    // int id = 0;

    // std::string server_name = servers[id]["server_name"];
    // ConfigValue listen = servers[id]["listen"];
    // int keepalive = servers[id]["keepalive_timeout"];

    // std::cout << "Server name: " << server_name << std::endl;
    // std::cout << "Listen: " << listen << std::endl;
    // std::cout << "Keepalive timeout: " << keepalive << std::endl;

    // std::cout << "--------------------------------" << std::endl;

    // config_array locations = config.getLocations(server_name);

    // for (size_t i = 0; i < locations.size(); i++) {
    //     ConfigValue v = locations[i];

    //     std::string root = v.getMap()["root"];
    //     config_array index = v.getMap()["index"];
        
    //     std::cout << "index: " << index << std::endl;
    //     std::cout << "Root: " << root << std::endl;

    //     for (size_t i = 0; i < index.size(); i++) {
    //         std::string c = index.at(i);
    //         std::cout << "Index " << i << " : " << c << std::endl;
    //     }

    //     // std::cout << locations[i] << std::endl;
    //     std::cout << "--------------------------------" << std::endl;
    // }

    return 0;
}
