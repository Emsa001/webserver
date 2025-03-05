#ifndef UTILS_HPP
#define UTILS_HPP

// String operations
void trim(std::string &s);
std::string intToString(int i);
std::vector<std::string> split(const std::string &s, char delim);

// File operations
std::string read_file(const std::string &path);
#endif