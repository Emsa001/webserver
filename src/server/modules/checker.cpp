/*

    This method is used to handle directory listing when the autoindex option is enabled.

*/

#include "Webserv.hpp"

bool Server::isDirectoryListing(const config_map *location, const FileData &fileData){
    bool autoIndex = false;
    if (location->find("autoindex") != location->end()) {
        autoIndex = location->at("autoindex").getBool();
    }

    return autoIndex;
}
