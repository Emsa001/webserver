/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: escura <escura@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:45:07 by escura            #+#    #+#             */
/*   Updated: 2025/02/08 19:40:00 by escura           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>

class ConfigValue;

typedef std::map<std::string, ConfigValue> config_map;
typedef std::vector<ConfigValue> config_array;

class ConfigValue {
    public:
        enum Type { INT, BOOL, STRING, ARRAY, MAP };

    private:
        Type type;

        int i;
        bool b;
        std::string s;
        config_array a;
        config_map m;

        void clear();

    public:
        ConfigValue();
        ConfigValue(int i);
        ConfigValue(bool b);
        ConfigValue(const std::string &str);
        ConfigValue(const config_array &array);
        ConfigValue(const config_map &map);
        ConfigValue(const ConfigValue &other);
        ConfigValue& operator=(const ConfigValue &other);
        ~ConfigValue();

        Type getType() const { return type; }
        int getInt() const { return (type == INT) ? i : 0; }
        bool getBool() const { return (type == BOOL) ? b : false; }
        std::string getString() const { return (type == STRING) ? s : ""; }
        config_array getArray() const { return (type == ARRAY) ? a : config_array(); }
        config_map& getMap() { return m; }
        const config_map& getMap() const { return m; }

        // Type conversions
        operator std::string() const;
        operator int() const;
        operator bool() const;
        operator config_array() const;
        operator config_map() const;

        bool operator==(const std::string &other) const;
        bool operator==(const int &other) const;
        bool operator==(const bool &other) const;

        void print() const;
        static ConfigValue detectType(const std::string &value);
};



std::ostream& operator<<(std::ostream& os, const ConfigValue& cv);

class Config
{
    private:        
        config_map root;

        // temp
        std::vector<config_map> blocks;
        config_map *block;
        int blockId;
        // temp

        std::ifstream file;
        std::string line;
        int lineNum;

    public:
        Config();
        Config(std::string const &filename);
        ~Config();

        void parse();
        void processLine();
        void setBlock(int level);

        void setKey(std::string const &key, std::string const &value);
        void updateParents();

        config_map getRoot() const { return root; }
};

#endif