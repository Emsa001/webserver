#include "Cgi.hpp"

StringMap Cgi::get_env(const std::string& scriptPath)
{
    StringMap env;

    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SCRIPT_FILENAME"] = scriptPath;
    env["REQUEST_METHOD"] = "GET";  // Modify based on request type
    env["QUERY_STRING"] = "";       // Extract from request URL
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
    while (i < env_map.size())
    {
        env[i] = new char[env_map.begin()->first.size() + env_map.begin()->second.size() + 2];
        strcpy(env[i], (env_map.begin()->first + "=" + env_map.begin()->second).c_str());
        env_map.begin()++;
        i++;
    }


    env[i] = NULL;
    return env;
}
