#include "Webserv.hpp"

Config::Config()
{
    this->block = NULL;
    this->blockId = 0;
    this->indent = 0;
    this->expectedIndent = -1;
    std::cout << "Config is created" << std::endl;
    this->file.open("conf/default.yml");
}

Config::Config(std::string const &filename)
{
    this->block = NULL;
    this->blockId = 0;
    std::cout << "Config is created" << std::endl;
    this->file.open(filename.c_str());
}

Config::~Config()
{
    std::cout << "Config destructor called, resources freed successfully." << std::endl;
}
