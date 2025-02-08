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

        this->processLine();
    }

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

void Config::processLine()
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
                return;
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
            if (c == '-')
                (*this->block)["blockType"] = ConfigValue(std::string("array"));

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

                return;
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

    // // Errors

    // if (currentIndent > 0 && this->block == NULL)
    // {
    //     std::cerr << "Error: indentation outside block" << std::endl;
    //     exit(1);
    // }

    // if (quote != '\0')
    // {
    //     std::cerr << "Error: quote not closed" << std::endl;
    //     exit(1);
    // }

    // if (key.empty())
    // {
    //     bool error = true;
    //     if(this->block != NULL){
    //         if(this->block->at("blockType").getString() == "array")
    //             error = false;
    //     }
    //     if(error){
    //         std::cerr << "Error: no key for line: '" << this->line << "'" << std::endl;
    //         exit(1);
    //     }
    // }

    // if (!isValue)
    // {
    //     std::cerr << "Error: no value for key: '" << key << "'" << std::endl;
    //     exit(1);
    // }

    this->setBlock(currentIndent - 4);
    setKey(key, value);
}

void Config::updateParents() {
    std::map<int, config_map*> parentMap; // Stores blockId -> pointer to block

    // Build the parent lookup table
    for (size_t i = 0; i < this->blocks.size(); i++) {
        int blockId = this->blocks[i].at("blockId").getInt();
        parentMap[blockId] = &this->blocks[i];
    }

    // Link children to their parents
    for (size_t i = 0; i < this->blocks.size(); i++) {
        config_map &block = this->blocks[i]; 
        int blockId = block.at("blockId").getInt();
        int level = block.at("level").getInt();

        if (blockId == 0) { 
            this->root[block.at("blockName")] = block;
            continue;
        }

        // Find parent by scanning backwards
        for (int j = blockId - 1; j >= 0; j--) {
            if (parentMap.find(j) != parentMap.end()) {
                config_map *parent = parentMap[j];
                if (parent->at("level").getInt() == level - 1) {
                    (*parent)[block.at("blockName")] = block; // Assign child to parent
                    break;
                }
            }
        }
    }
}


void Config::setKey(const std::string &key, const std::string &value) {
    ConfigValue typedValue = value;

    if (this->block != NULL) {
        std::string blockName = this->block->at("blockName");
        int blockId = this->block->at("blockId");
        int level = this->block->at("level").getInt();

        (*this->block)[key] = typedValue;

        if (blockId == 0) return;

        config_map* parent = NULL;
        for (int i = blockId - 1; i >= 0; i--) {
            if (this->blocks[i].at("level").getInt() == level - 1) {
                parent = &this->blocks[i];
                break;
            }
        }

        if (parent) {
            (*parent)[blockName] = *this->block; 
            
            // config_map* currentParent = parent;
            // while (currentParent) {
            //     std::string parentName = currentParent->at("blockName");
            //     int parentId = currentParent->at("blockId");
                
            //     config_map* grandParent = NULL;
            //     for (int j = parentId - 1; j >= 0; j--) {
            //         if (this->blocks[j].at("level").getInt() == currentParent->at("level").getInt() - 1) {
            //             grandParent = &this->blocks[j];
            //             break;
            //         }
            //     }

            //     if (grandParent) {
            //         (*grandParent)[parentName] = *currentParent;
            //         currentParent = grandParent;
            //     } else {
            //         currentParent = NULL;
            //     }
            // }
        }

        std::cout << "Block : " << *this->block << std::endl;
        std::cout << "Parent: " << *parent << std::endl;

        updateParents();
        return;
    }

    this->root[key] = typedValue;
}
