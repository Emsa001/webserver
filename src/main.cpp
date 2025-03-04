#include "Webserv.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <map>


void* startServer(void* arg) {
    config_map* data = static_cast<config_map*>(arg);
    Server server(*data);
    server.start();
    return NULL;
}

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

    std::vector<pthread_t> threads;

    for(; it != servers.end(); it++){
        std::string server_name = it->getMap()["server_name"];
        std::cout << "Server name: " << server_name << std::endl;


        pthread_t thread;
        pthread_create(&thread, NULL, startServer, &it->getMap());
        threads.push_back(thread);
    }

    for(size_t i = 0; i < threads.size(); i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

