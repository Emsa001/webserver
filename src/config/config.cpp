#include "Webserv.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

bool Config::parse()
{
    if (!this->file.is_open())
    {
        std::cerr << "Error: Could not open file" << std::endl;
        return false;
    }

    while (std::getline(this->file, this->line))
    {
        this->ln++;
        if (this->line.empty())
            continue;

        try{
            if(!this->processLine()) break;
        }catch(const std::exception &e){
            this->file.close();
            std::cerr << e.what() << std::endl;
            this->root.clear();
            this->blocks.clear();
            return false;
        }
    }

    this->cleanTemp(&this->root);
    this->file.close();

    return true;
}

bool Config::isReserved(const std::string &key){
    return (key == "blockId" || key == "blockName" || key == "blockType" || key == "level");
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