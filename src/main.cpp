#include "Webserv.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <map>


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
    Config config("conf/myserver.yml");
    if(!config.parse())
        return 1;

    config_array servers = config.getServers();
    config_map first = servers[0];

    Server server1(first);
    server1.run();

    // config_array::iterator it = servers.begin();

    // std::vector<pthread_t> threads;

    // for(; it != servers.end(); it++){
    //     std::string server_name = it->getMap()["server_name"];
    //     std::cout << "Server name: " << server_name << std::endl;


    //     pthread_t thread;
    //     pthread_create(&thread, NULL, startServer, &it->getMap());
    //     threads.push_back(thread);
    // }

    // for(size_t i = 0; i < threads.size(); i++) {
    //     pthread_join(threads[i], NULL);
    // }

    return 0;
}