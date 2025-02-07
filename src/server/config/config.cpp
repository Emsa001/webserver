#include "webserv.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

bool find(const std::string &str, const std::string &to_find)
{
    return str.find(to_find) != std::string::npos;
}

void erase(std::string &str, const std::string &to_erase)
{
    size_t pos = str.find(to_erase);

    if (pos != std::string::npos)
    {
        str.erase(pos, to_erase.size());
    }
}

Config::Config()
{
    this->block = NONE;
    std::cout << "Config is created" << std::endl;

    this->file.open("conf/default.conf");

    if (!this->file.is_open())
    {
        std::cerr << "Error: Could not open file" << std::endl;
        exit(1);
    }

    std::cout << std::endl
              << std::endl;
    this->lineNum = 1;
    while (std::getline(this->file, this->line))
    {
        this->lineNum++;
        trim(this->line);

        if (this->line[0] == '#' || this->line.empty())
            continue;

        setBlock();
    }

    if (this->block != NONE)
    {
        std::cerr << "Error: block not closed" << std::endl;
        exit(1);
    }

    this->file.close();
}

Config::~Config() {
    for (size_t i = 0; i < servers.size(); i++) {
        servers[i].clear();
    }

    std::cout << "Config destructor called, resources freed successfully." << std::endl;
}

std::ostream& operator<<(std::ostream& os, const ConfigValue& cv) {
    switch (cv.getType()) {
        case ConfigValue::INT:
            os << cv.getInt();
            break;
        case ConfigValue::BOOL:
            os << (cv.getBool() ? "true" : "false");
            break;
        case ConfigValue::STRING:
            os << cv.getString();
            break;
        case ConfigValue::ARRAY:
            os << "[ ";
            for (size_t i = 0; i < cv.getArray().size(); i++) {
                if (i > 0) os << ", ";
                os << (cv.getArray())[i];
            }
            os << " ]";
            break;
        case ConfigValue::MAP:
            os << "{ ";
            for (config_map::const_iterator it = cv.getMap().begin(); it != cv.getMap().end(); ++it) {
                if (it != cv.getMap().begin()) os << ", ";
                os << it->first << ": " << it->second;
            }
            os << " }";
            break;
    }
    return os;
}


// Member functions


void Config::setBlock()
{
    std::string &line = this->line;

    int temp = this->block;

    if (line == "server")
    {
        if (this->block != NONE)
        {
            std::cerr << "Error: server block wrong position" << std::endl;
            exit(1);
        }
        this->parent = this->block;
        this->block = SERVER_BLOCK;
        this->servers.push_back(config_map());
        getline(this->file, this->line);
    }
    else if (line.compare(0, 8, "location") == 0)
    {
        if (this->block != SERVER_BLOCK)
        {
            std::cerr << "Error: location block wrong position" << std::endl;
            exit(1);
        }

        this->parent = this->block;
        this->block = LOCATION_BLOCK;
    }
    else if (this->block == NONE)
    {
        std::cerr << "Error: block not found: " << line << " " << lineNum << std::endl;
        exit(1);
    }
    else if (line == "}")
    {
        this->block = this->parent;
        this->parent = NONE;
    }

    readBlock();
}

void Config::readBlock()
{

    if (this->line == "{" || this->line == "}")
        return;

    config_map *map = NULL;

    std::string key;
    std::string value;

    std::size_t first_space = this->line.find_first_of(' ');

    if (first_space != std::string::npos)
    {
        key = this->line.substr(0, first_space);

        std::size_t end = this->line.find_first_of(';', first_space);
        if (end != std::string::npos)
            value = this->line.substr(first_space, end - first_space);
        else
            value = this->line.substr(first_space);
    }

    trim(value);
    loadKey(key, value);
}

void Config::loadKey(const std::string &key, const std::string &value)
{
    ConfigValue typedValue = ConfigValue::detectType(value);

    if (this->block == SERVER_BLOCK)
        this->servers.back()[key] = ConfigValue(typedValue);
    else if (this->block == LOCATION_BLOCK)
    {
        if (key == "location") {
            this->currentLocation = getFirstWord(value);
            this->servers.back()[this->currentLocation] = ConfigValue(config_map());
            config_map &locationMap = this->servers.back()[this->currentLocation].getMap();
            locationMap["type"] = key;
            locationMap["path"] = this->currentLocation;
        } else {
            config_map &locationMap = this->servers.back()[this->currentLocation].getMap();
            locationMap[key] = typedValue;
        }
    }
}

config_servers Config::getServers() {
    return this->servers;
}

config_array Config::getLocations(std::string const &server_name) {
    config_array locations;

    for (size_t i = 0; i < this->servers.size(); i++) {
        if (this->servers[i]["server_name"] == server_name) {
            config_map m = this->servers[i];

            for (config_map::iterator it = m.begin(); it != m.end(); it++) {
                if(it->second.getType() == ConfigValue::MAP){
                    locations.push_back(it->second);
                }
            }
        }
    }

    return locations;
}