#include "Cgi.hpp"

StringMap Cgi::get_env(const std::string& scriptPath, std::string query)
{
    StringMap env;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SCRIPT_FILENAME"] = scriptPath;
    env["REQUEST_METHOD"] = "GET";  // Modify based on request type
    env["QUERY_STRING"] = query;       // Extract from request URL
    env["CONTENT_LENGTH"] = "";     // Required for POST
    env["CONTENT_TYPE"] = "";       // Required for POST
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "MyWebServer/1.0";

    return env;
}

char **Cgi::convert_env(const StringMap& env_map)
{
    char **env = new char*[env_map.size() + 1];
    size_t i = 0;

    for (StringMap::const_iterator it = env_map.begin(); it != env_map.end(); ++it)
    {
        std::string entry = it->first + "=" + it->second;
        env[i] = new char[entry.size() + 1];
        std::strcpy(env[i], entry.c_str());
        ++i;
    }

    env[i] = NULL;
    return env;
}

