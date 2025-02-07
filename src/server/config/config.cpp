#include "webserv.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

Config::Config()
{
    this->block = NULL;
    this->blockId = 0;
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

Config::~Config() {
    std::cout << "Config destructor called, resources freed successfully." << std::endl;
}


// Member functions

void Config::parse(){
    if (!this->file.is_open())
    {
        std::cerr << "Error: Could not open file" << std::endl;
        return ;
    }

    std::cout << std::endl
              << std::endl;
    this->lineNum = 0;
    while (std::getline(this->file, this->line))
    {
        this->lineNum++;
        if (this->line.empty())
            continue;

        this->processLine();
    }

    this->file.close();
}

void Config::setBlock(int currentIndent) {
    if(this->block == NULL)
        return ;

    while (!this->blocks.empty() && currentIndent < this->blocks.top().at("level").getInt() * 4) {
        this->blocks.pop();
    }

    if(this->blocks.empty())
        this->block = NULL;
    else{
        this->block = &(this->blocks.top());
        int blockIndent = this->block->at("level").getInt() * 4;

        if(blockIndent != currentIndent) {
            std::cerr << "Error: indentation not matching" << std::endl;
            exit(1);
        }
    }
}


void Config::processLine() {

    std::string &line = this->line;
    char quote = '\0';

    std::string key;
    std::string value;
    bool isValue = false;

    int currentIndent = 0;

    for (size_t i = 0; i < line.size(); i++) {
        char p = i > 0 ? line[i - 1] : '\0';
        char c = line[i];
        char n = line.size() > i + 1 ? line[i + 1] : '\0';

        // menage comments
        if(c == '#' && (std::isspace(p) || i == 0) && quote == '\0'){
            if(i == 0)
                return ;
            break;
        }

        // manage intent
        if (std::isspace(c) && quote == '\0' && key.empty()) {
            currentIndent++;
            continue;
        }
        
        // manage quotes
        if (c == '"' || c == '\'') {
            if(quote == '\0')
                quote = c;
            else if (quote == c)
                quote = '\0';
            continue;
        }

        if(c == ':' && !isValue){
            if(n == '\0' || n == '\n'){
                config_map newBlock = config_map();
                const int level = currentIndent / 4;

                newBlock["blockType"] = ConfigValue(key);
                newBlock["level"] = ConfigValue(level);
                newBlock["blockId"] = ConfigValue(this->blockId++);

                this->blocks.push(newBlock);
                this->block = &(blocks.top());
                
                if(level == 0){
                    this->servers.push_back(newBlock);
                }

                return ;
            }
            else if(std::isspace(n)){
                isValue = true;
                i++;
                continue;
            }
        }

        if(isValue) {
            value += c;
        } else {
            key += c;
        }
    }
    
    this->setBlock(currentIndent - 4);

    // Errors

    if(currentIndent > 0 && this->block == NULL) {
        std::cerr << "Error: indentation outside block" << std::endl;
        exit(1);
    }

    if(quote != '\0') {
        std::cerr << "Error: quote not closed" << std::endl;
        exit(1);
    }

    if(key.empty()) {
        std::cerr << "Error: no key" << std::endl;
        exit(1);
    }

    if(!isValue) {
        std::cerr << "Error: no value" << std::endl;
        exit(1);
    }

    setKey(key, value);
}

void Config::setKey(const std::string &key, const std::string &value)
{
    ConfigValue typedValue = ConfigValue::detectType(value);

    if (this->block != NULL){
        std::string type = this->block->at("blockType");
        if (type == "location") {
            config_array locations = this->servers.back().getMap()["locations"];
            bool blockExists = false;

            for (config_array::iterator it = locations.begin(); it != locations.end(); ++it) {
                int blockId = it->getMap()["blockId"];
                int currentBlockId = this->block->at("blockId");
                if(blockId == currentBlockId) {
                    it->getMap()[key] = typedValue;
                    blockExists = true;
                    break;
                }
            }

            if (!blockExists) {
                locations.push_back(*this->block);
            }

            this->servers.back().getMap()["locations"] = locations;
            return;
        }
        this->servers.back().getMap()[key] = typedValue;
    }
    else
        this->root[key] = typedValue;
}