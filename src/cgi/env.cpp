#include "Cgi.hpp"

std::string get_query(const std::string &uri)
{
    size_t pos = uri.find('?');
    if (pos != std::string::npos)
    {
        return uri.substr(pos + 1);
    }
    return "";
}

StringMap Cgi::get_env(const std::string& scriptPath, const HttpRequest& request)
{
    std::string query = get_query(request.getURI());

    StringMap env;
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SCRIPT_FILENAME"] = scriptPath;
    env["REQUEST_METHOD"] = request.getMethod();
    env["QUERY_STRING"] = query;
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "MyWebServer/1.0";

    if (request.getMethod() == "POST" || request.getMethod() == "DELETE")
    {
        env["CONTENT_LENGTH"] = intToString(request.getBody().size());
        std::string contentType = request.getHeader("Content-Type");
        if (!contentType.empty())
            env["CONTENT_TYPE"] = contentType;
    }

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

