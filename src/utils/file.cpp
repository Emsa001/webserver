#include "Webserv.hpp"

std::string read_file(const std::string &path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file) return "";
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}