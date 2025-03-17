#include "Cgi.hpp"

bool ends_with(const std::string &str, const std::string &suffix)
{
    if(str.size() >= suffix.size())
        if(str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0)
            return true;
    return false;
}

Cgi::Type Cgi::detect_type(const std::string &scriptPath)
{
    if (ends_with(scriptPath, ".py"))
        return PYTHON;
    if (ends_with(scriptPath, ".php"))
        return PHP;
    if (ends_with(scriptPath, ".sh"))
        return SHELL;
    return UNKNOWN;
}

std::string Cgi::get_interpreter(Type type)
{
    switch (type)
    {
        case PYTHON:
            return "/usr/bin/python3";
        case PHP:
            return "/usr/bin/php";
        case SHELL:
            return "/bin/sh";
        default:
            return "";
    }
}
