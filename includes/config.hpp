/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: escura <escura@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 14:45:07 by escura            #+#    #+#             */
/*   Updated: 2025/02/07 15:44:21 by escura           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <cstdlib>

class ConfigValue;

typedef std::map<std::string, ConfigValue> config_map;
typedef std::vector<ConfigValue> config_array;
typedef std::vector<config_map> config_servers;

class ConfigValue {
    public:
        enum Type { INT, BOOL, STRING, ARRAY, MAP };

    private:
        Type type;
        union {
            int i;
            bool b;
            std::string* s;
            config_array* a;
            config_map* m;
        };

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
        std::string getString() const { return (type == STRING) ? *s : ""; }
        config_array getArray() const { return (type == ARRAY) ? *a : config_array(); }
        config_map& getMap() { return *m; }
        const config_map& getMap() const { return *m; }

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


std::ostream& operator<<(std::ostream& os, const ConfigValue& cv);

class Config
{
    private:
        enum BlockType { NONE, SERVER_BLOCK, LOCATION_BLOCK };
        
        config_servers servers;

        // temp
        BlockType parent;
        BlockType block;
        std::string currentLocation;
        // temp

        std::ifstream file;
        std::string line;
        int lineNum;

    public:
        Config();
        ~Config();

        void setBlock();
        void readBlock();
        void loadKey(std::string const &key, std::string const &value);

        config_servers getServers();
        config_array getLocations(std::string const &server_name);
};

#endif