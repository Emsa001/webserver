#include "webserv.hpp"

int main()
{
    handle_signal();

    Config config;
    config.parse();

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    config_map root = config.getRoot();
    std::cout << root << std::endl;

    config_array servers = config.getServers();
    std::cout << servers << std::endl;

    for (size_t i = 0; i < servers.size(); i++) {
        ConfigValue v = servers[i];

        std::cout << v << std::endl;

        std::string server_name = v.getMap()["logFormat"];
        ConfigValue locations = v.getMap()["locations"];
        // std::string listen = v.getMap()["listen"];
        // std::string keepalive = v.getMap()["keep_alive_timeout"];
        // std::string max_size = v.getMap()["max_client_body_size"];

        std::cout << "Server name: " << server_name << std::endl;
        std::cout << "Locations: " << locations << std::endl;

        std::cout << "--------------------------------" << std::endl;
    }

    // int id = 0;
    // config_servers servers = config.getServers();

    // ConfigValue server_name = servers[id]["server_name"];
    // ConfigValue listen      = servers[id]["listen"];
    // ConfigValue keepalive   = servers[id]["keep_alive_timeout"];
    // ConfigValue max_size    = servers[id]["max_client_body_size"];

    // std::cout << "--------------------------------" << std::endl;

    // std::cout << "Server name: " << server_name << std::endl;
    // std::cout << "Listen: " << listen << std::endl;
    // std::cout << "Keepalive timeout: " << keepalive << std::endl;
    // std::cout << "Max client body size: " << max_size << std::endl;

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


/*

{ 
    logFormat: [ [request], [status], [time] ], 
    max_thread: 3310 
}


[ 
    { 
        blockId: 0, 
        blockType: server, 
        level: 0, 

        listen: 8080, 
        locations: [ 
            { 
                blockId: 1, 
                blockType: location, 
                level: 1, 

                index: index.html, 
                root: /var/www/static 
            }, 
            { 
                blockId: 2, 
                blockType: location, 
                level: 1, 
                
                index: index.jpg, 
                root: /var/www/images 
            } 
        ], 
        max_client_body_size: 1000, 
        server_name: localhost 
    }, 
    
    { 
        blockId: 3, 
        blockType: server, 
        level: 0, 

        listen: 8081, 
        max_client_body_size: 1000, 
        server_name: example.com 
    } 
]

*/