#include "Webserv.hpp"

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
    serverSchema.addEntry("server_name", STRING, true);
    serverSchema.addEntry("listen", INT, true);
    serverSchema.addEntry("max_client_body_size", INT, true);
    serverSchema.addEntry("keep_alive", INT, false);

    rootSchema.addNestedSchema("server", serverSchema);

    // Location Schema
    ConfigSchema locationSchema;
    locationSchema.addEntry("path", STRING, true);
    locationSchema.addEntry("exact", BOOL, false);
    locationSchema.addEntry("methods", STRING, false); // TODO: implement array? (I'm not doing it, too lazy)
    locationSchema.addEntry("root", STRING, false);
    locationSchema.addEntry("index", STRING, false);
    locationSchema.addEntry("redirect", STRING, false);
    locationSchema.addEntry("autoindex", BOOL, false);
    locationSchema.addEntry("upload_path", STRING, false);

    locationSchema.addEntry("cgi", BOOL, false);

    rootSchema.addNestedSchema("location", locationSchema);

    // Error Schema
    ConfigSchema errorSchema;
    errorSchema.addEntry("404", STRING, true);
    errorSchema.addEntry("403", STRING, false);
    errorSchema.addEntry("500", STRING, false);

    rootSchema.addNestedSchema("errors", errorSchema);

    return rootSchema;
}


/*

Execute CGI based on certain file extension ?
Because you won’t call the CGI directly, use the full path as ?


*/