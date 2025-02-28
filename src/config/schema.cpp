#include "Config.hpp"

ConfigSchema::ConfigSchema() {}

void ConfigSchema::addEntry(const std::string &key, ValueType type, bool required) {
    SchemaEntry entry;
    entry.type = type;
    entry.required = required;
    schema[key] = entry;
}
void ConfigSchema::addNestedSchema(const std::string &key, const ConfigSchema &nestedSchema) {
    nestedSchemas[key] = nestedSchema;
}

ConfigSchema createSchema() {
    ConfigSchema rootSchema;

    rootSchema.addEntry("log_format", STRING, true);

    // Server Schema
    ConfigSchema serverSchema;
    serverSchema.addEntry("listen", INT, true);
    serverSchema.addEntry("max_client_body_size", INT, true);

    rootSchema.addNestedSchema("server", serverSchema);

    // Location Schema
    ConfigSchema locationSchema;
    locationSchema.addEntry("path", STRING, true);
    locationSchema.addEntry("root", STRING, true);
    locationSchema.addEntry("index", STRING, false);
    locationSchema.addEntry("autoindex", BOOL, false);

    rootSchema.addNestedSchema("location", locationSchema);

    // Error Schema
    ConfigSchema errorSchema;
    errorSchema.addEntry("404", STRING, true);
    errorSchema.addEntry("403", STRING, false);
    errorSchema.addEntry("500", STRING, false);

    rootSchema.addNestedSchema("errors", errorSchema);

    return rootSchema;
}

bool ConfigSchema::validate(const std::string &key, ValueType type, int blockKind) const {
    const SchemaMap *validSchema;

    if(Config::isReserved(key)) return true;

    switch(blockKind){
        case SERVER:
            if (nestedSchemas.find("server") != nestedSchemas.end()) {
                validSchema = &(nestedSchemas.find("server")->second.schema);
            }
            break;
        case LOCATION:
            if (nestedSchemas.find("location") != nestedSchemas.end()) {
                validSchema = &(nestedSchemas.find("location")->second.schema);
            }
            break;
        case ERRORS:
            if (nestedSchemas.find("errors") != nestedSchemas.end()) {
                validSchema = &(nestedSchemas.find("errors")->second.schema);
            }
            break;
        default:
            validSchema = &this->schema;
            break;
    }

    if (validSchema->find(key) == validSchema->end()) {
        return false;
    }
    if (validSchema->find(key)->second.type != type) {
        return false;
    }

    return true;
}

bool ConfigSchema::validateMap(config_map &map) const {
    int blockKind = map.find("blockKind")->second.getInt();

    if(map.find("locations") != map.end()){
        config_array locations = map.find("locations")->second.getArray();
        for(size_t i = 0; i < locations.size(); i++){
            config_map location = locations[i].getMap();
            if(this->validateMap(location) == false){
                return false;
            }
        }
    }

    if(map.find("errors") != map.end()){
        config_map errors = map.find("errors")->second.getMap();
        if(this->validateMap(errors) == false){
            return false;
        }
    }

    std::string schemaName;
    switch(blockKind){
        case SERVER:
            schemaName = "server";
            break;
        case LOCATION:
            schemaName = "location";
            break;
        case ERRORS:
            schemaName = "errors";
            break;
        default:
            schemaName = "root";
            break;
    }

    SchemaMap::const_iterator it;
    for (it = nestedSchemas.find(schemaName)->second.schema.begin(); it != nestedSchemas.find(schemaName)->second.schema.end(); ++it) {
        if (it->second.required && map.find(it->first) == map.end()) {
            std::cerr << "Error: Required key '" << it->first << "' not found" << std::endl;
            return false;
        }        
    }

    return true;
}

bool ConfigSchema::validateRequired(const Config *config) const {

    config_map root = config->getRoot();
    config_array servers = config->getServers();

    SchemaMap::const_iterator it;
    for (it = schema.begin(); it != schema.end(); ++it) {
        if (it->second.required && root.find(it->first) == root.end()) {
            std::cerr << "Error: Required key '" << it->first << "' not found" << std::endl;
            return false;
        }
    }


    for(size_t i = 0; i < servers.size(); i++){
        config_map server = servers[i].getMap();

        if(this->validateMap(server) == false){
            return false;
        }
    }

    return true;
}