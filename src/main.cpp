#include "webserv.hpp"
#include "../ft_async/includes/ft_async.h"

void runServer(t_async *async)
{
    int port = (intptr_t)async->arg;

    Server server(port);
    server.run();
}

int main()
{
    handle_signal();

    Config config;

    // config_servers::iterator it = servers.begin();
    // for (it; it != servers.end(); it++)
    // {
    //     int port = it->at("listen");

    //     t_async *async = new_async();
    //     async->time = -1;
    //     async->start = runServer;
    //     async->arg = (void *)(intptr_t)port;
    //     start_async(async);

    // }

    // destroy_manager();
    int id = 0;

    config_servers servers = config.getServers();
    std::string server_name = servers[id]["server_name"];
    ConfigValue listen = servers[id]["listen"];
    int keepalive = servers[id]["keepalive_timeout"];

    std::cout << "Server name: " << server_name << std::endl;
    std::cout << "Listen: " << listen << std::endl;
    std::cout << "Keepalive timeout: " << keepalive << std::endl;

    std::cout << "--------------------------------" << std::endl;

    config_array locations = config.getLocations(server_name);

    for (size_t i = 0; i < locations.size(); i++) {
        ConfigValue v = locations[i];

        std::string root = v.getMap()["root"];
        config_array index = v.getMap()["index"];

        std::cout << "index: " << index << std::endl;
        std::cout << "Root: " << root << std::endl;

        for (size_t i = 0; i < index.size(); i++) {
            std::string c = index.at(i);
            std::cout << "Index " << i << " : " << c << std::endl;
        }

        // std::cout << locations[i] << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }

    destroy_manager();
    return 0;
}
