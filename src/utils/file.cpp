#include "Webserv.hpp"

std::string readFile(const std::string &path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool isDirectory(const std::string &path) {
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        return true;
    }
    return false;
}

bool fileExists(const std::string &path) {
    struct stat sb;
    return stat(path.c_str(), &sb) == 0;
}