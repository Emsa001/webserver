#include "webserv.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>


// Member functions

void Config::parse()
{
    if (!this->file.is_open())
    {
        std::cerr << "Error: Could not open file" << std::endl;
        return;
    }

    while (std::getline(this->file, this->line))
    {
        if (this->line.empty())
            continue;

        if(!this->processLine()) break;
    }

    this->cleanTemp(&this->root);
    this->file.close();
}

bool Config::setBlock()
{
    if (this->block == NULL)
        return true;

    while (!this->blocks.empty() && (this->indent - 4) < this->blocks.back().at("level").getInt() * 4)
        this->blocks.pop_back();

    if (this->blocks.empty()){
        this->block = NULL;
        this->indent = 0;
        this->blocks.clear();
        return true;
    }
    
    this->block = &(this->blocks.back());
    const int blockIndent = this->block->at("level").getInt() * 4;

    if (blockIndent != this->indent - 4)
        throw std::runtime_error("Error: indentation not matching");

    return true;
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
    return (key == "blockId" || key == "blockName" || key == "blockType" || key == "level");
}

bool Config::setKey(const std::string &key, const std::string &value) {
    ConfigValue typedValue = ConfigValue::detectType(value);
    
    if (isReserved(key)) {
        std::cerr << "Error: " << key << " is reserved" << std::endl;
        return false;
    }
    
    if (this->block != NULL)
        return setKeyInBlock(key, typedValue);
    
    this->root[key] = typedValue;

    return true;
}

bool Config::setKeyInBlock(const std::string &key, const ConfigValue &typedValue) {

    std::cout << *this->block << std::endl;

    std::string blockName = this->block->at("blockName");
    int blockId = this->block->at("blockId");
    int level = this->block->at("level").getInt();
    
    if (level > 1) {
        std::cerr << "Error: block level too deep" << std::endl;
        return false;
    }

    
    (*this->block)[key] = typedValue;
    config_map* parent = findParentBlock(blockId, level);
    
    if (parent)
        updateParentBlock(parent, blockName, blockId);
    
    updateParents();
    return true;
}

config_map* Config::findParentBlock(int blockId, int level) {
    config_map* parent = &this->root;

    for (int i = blockId - 1; i >= 0; i--) {
        if (this->blocks[i].find("level") != this->blocks[i].end() && this->blocks[i].find("level")->second.getInt() == level - 1) {
            return &this->blocks[i];
        }
    }
    return parent;
}

void Config::updateParentBlock(config_map* parent, const std::string &blockName, int blockId) {
    if (blockName == "location") {
        if (!parent->count("locations"))
            (*parent)["locations"] = ConfigValue(config_array());
        config_array locations = (*parent)["locations"];
        bool blockIdExists = false;
        
        for (size_t i = 0; i < locations.size(); ++i) {
            if (locations[i].getMap().at("blockId").getInt() == blockId) {
                locations[i] = *this->block;
                blockIdExists = true;
                break;
            }
        }
        
        if (!blockIdExists)
            locations.push_back(*this->block);
        
        (*parent)["locations"] = locations;

        return ;
    }

    (*parent)[blockName] = *this->block;
}

config_map Config::cleanTemp(config_map *temp) {
    config_map::iterator it = temp->begin();
    
    while(it != temp->end()){
        if (it->second.getType() == ConfigValue::MAP)
            cleanTemp(&(it->second.getMap()));

        if(it->second.getType() == ConfigValue::ARRAY){
            config_array array = it->second.getArray();
            config_array temp;

            for(size_t i = 0; i < array.size(); i++){
                if(array[i].getType() == ConfigValue::MAP)
                    temp.push_back(cleanTemp(&(array[i].getMap())));
            }
            it->second = ConfigValue(temp);
        }

        if (this->isReserved(it->first)) temp->erase(it++);
        else it++;
    }

    return *temp;
}