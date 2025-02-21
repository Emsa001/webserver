#include "Config.hpp"

void Config::createNewBlock(const std::string &key)
{
    config_map newBlock = config_map();
    int level = this->indent / 4;
    newBlock["blockName"] = ConfigValue(key);
    newBlock["level"] = ConfigValue(level);
    newBlock["blockId"] = ConfigValue(this->blockId++);
    newBlock["blockType"] = ConfigValue(std::string("map"));
    this->blocks.push_back(newBlock);
    this->block = &(blocks.back());
}

void Config::setBlock()
{
    if (this->block == NULL)
        return;

    while (!this->blocks.empty() && (this->indent - 4) < this->blocks.back().at("level").getInt() * 4)
        this->blocks.pop_back();

    if (this->blocks.empty()){
        this->block = NULL;
        this->indent = 0;
        this->blocks.clear();
        return;
    }
    
    this->block = &(this->blocks.back());
    const int blockIndent = this->block->at("level").getInt() * 4;

    if (blockIndent != this->indent - 4)
        throw ParseError(this->ln, "Indentation not matching");

    return;
}

bool Config::setKeyInBlock(const std::string &key, const ConfigValue &typedValue) {

    std::string blockName = this->block->at("blockName");
    int blockId = this->block->at("blockId");
    int level = this->block->at("level").getInt();
    
    if (level > 1)
        throw std::runtime_error("Error: block level too deep");

    
    (*this->block)[key] = typedValue;
    config_map* parent = findParentBlock(blockId, level);
    
    if (parent)
        updateParentBlock(parent, blockName, blockId);
    
    updateParents();
    return true;
}
