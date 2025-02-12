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

Config::~Config()
{
    std::cout << "Config destructor called, resources freed successfully." << std::endl;
}

// Member functions

void Config::parse()
{
    if (!this->file.is_open())
    {
        std::cerr << "Error: Could not open file" << std::endl;
        return;
    }

    std::cout << std::endl
              << std::endl;
    this->lineNum = 0;
    while (std::getline(this->file, this->line))
    {
        this->lineNum++;
        if (this->line.empty())
            continue;

        if(this->processLine()) break;
    }

    this->cleanTemp(&this->root);
    this->file.close();
}

void Config::setBlock(int currentIndent)
{
    if (this->block == NULL)
        return;

    while (!this->blocks.empty() && currentIndent < this->blocks.back().at("level").getInt() * 4)
    {
        this->blocks.pop_back();
    }

    if (this->blocks.empty())
        this->block = NULL;
    else
    {
        this->block = &(this->blocks.back());
        int blockIndent = this->block->at("level").getInt() * 4;

        if (blockIndent != currentIndent)
        {
            std::cerr << "Error: indentation not matching" << std::endl;
            exit(1);
        }
    }
}

bool Config::processLine()
{

    std::string &line = this->line;
    char quote = '\0';

    std::string key;
    std::string value;
    bool isValue = false;

    int currentIndent = 0;

    for (size_t i = 0; i < line.size(); i++)
    {
        char p = i > 0 ? line[i - 1] : '\0';
        char c = line[i];
        char n = line.size() > i + 1 ? line[i + 1] : '\0';

        // menage comments
        if (c == '#' && (std::isspace(p) || i == 0) && quote == '\0')
        {
            if (i == 0)
                return 0;
            break;
        }

        // manage intent
        if (std::isspace(c) && quote == '\0' && key.empty())
        {
            currentIndent++;
            continue;
        }

        // manage quotes
        if (c == '"' || c == '\'')
        {
            if (quote == '\0')
                quote = c;
            else if (quote == c)
                quote = '\0';
            continue;
        }

        if ((c == ':' || c == '-') && !isValue)
        {
            // if (c == '-')
            //     (*this->block)["blockType"] = ConfigValue(std::string("array"));

            if (n == '\0' || n == '\n')
            {
                config_map newBlock = config_map();
                int level = currentIndent / 4;

                newBlock["blockName"] = ConfigValue(key);
                newBlock["level"] = ConfigValue(level);
                newBlock["blockId"] = ConfigValue(this->blockId++);
                newBlock["blockType"] = ConfigValue(std::string("map"));

                this->blocks.push_back(newBlock);
                this->block = &(blocks.back());

                return 0;
            }
            else if (std::isspace(n))
            {
                isValue = true;
                i++;
                continue;
            }
        }

        if (isValue)
        {
            value += c;
        }
        else
        {
            key += c;
        }
    }

    // Errors

    if (currentIndent > 0 && this->block == NULL)
    {
        std::cerr << "Error: indentation outside block" << std::endl;
        return 1;
    }

    if (quote != '\0')
    {
        std::cerr << "Error: quote not closed" << std::endl;
        return 1;
    }

    if (key.empty())
    {
        bool error = true;
        if(this->block != NULL){
            if(this->block->at("blockType").getString() == "array")
                error = false;
        }
        if(error){
            std::cerr << "Error: no key for line: '" << this->line << "'" << std::endl;
            return 1;
        }
    }

    if (!isValue)
    {
        std::cerr << "Error: no value for key: '" << key << "'" << std::endl;
        return 1;
    }

    this->setBlock(currentIndent - 4);
    return setKey(key, value);
}

void Config::updateParents() {
    for (size_t i = 0; i < this->blocks.size(); i++) {
        config_map &block = this->blocks[i]; 
        int blockId = block.at("blockId").getInt();
        int level = block.at("level").getInt();

        if (blockId == 0) { 
            this->root[block.at("blockName")] = block;
            continue;
        }
    }
}

bool Config::isReserved(const std::string &key){
    if(key == "blockId" || key == "blockName" || key == "blockType" || key == "level") {
        return true;
    }
    return false;
}


bool Config::setKey(const std::string &key, const std::string &value) {
    ConfigValue typedValue = ConfigValue::detectType(value);

    if(this->isReserved(key)){
        std::cerr << "Error: " << key << " is reserved" << std::endl;
        return 1;
    }

    if (this->block != NULL) {
        std::string blockName = this->block->at("blockName");
        int blockId = this->block->at("blockId");
        int level = this->block->at("level").getInt();

        if(level > 1){
            std::cerr << "Error: block level too deep" << std::endl;
            return 1;
        }

        (*this->block)[key] = typedValue;


        config_map* parent = &this->root;
        for (int i = blockId - 1; i >= 0; i--) {
            if (this->blocks[i].at("level").getInt() == level - 1) {
                parent = &this->blocks[i];
                break;
            }
        }

        if (parent){
            if (blockName == "location") {
                if (!parent->count("locations"))
                    (*parent)["locations"] = ConfigValue(config_array());
                
                config_array locations = (*parent)["locations"];
                bool blockIdExists = false;

                for (size_t i = 0; i < locations.size(); ++i) {
                    if(locations[i].getMap().at("blockId").getInt() == blockId){                        
                        locations[i] = *this->block;
                        blockIdExists = true;
                        break;
                    }
                }

                if (!blockIdExists)
                    locations.push_back(*this->block);

                parent->at("locations") = locations;

            } else
                (*parent)[blockName] = *this->block;
        }

        updateParents();
        return 0;
    }

    this->root[key] = typedValue;

    return 0;
}

config_map Config::cleanTemp(config_map *temp) {
    config_map::iterator it = temp->begin();
    
    while(it != temp->end()){
        
        if (it->second.getType() == ConfigValue::MAP) {
            cleanTemp(&(it->second.getMap()));
        }
        if(it->second.getType() == ConfigValue::ARRAY){
            config_array array = it->second.getArray();
            config_array test;
            for(size_t i = 0; i < array.size(); i++){
                if(array[i].getType() == ConfigValue::MAP)
                    test.push_back(cleanTemp(&(array[i].getMap())));
            }
            it->second = ConfigValue(test);
        }
        
        if (this->isReserved(it->first)) {
            config_map::iterator tempIt = it;
            ++it;
            temp->erase(tempIt);
        } else {
            ++it;
        }
    }

    return *temp;
}