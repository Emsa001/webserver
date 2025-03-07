#ifndef UTILS_HPP
#define UTILS_HPP

// String operations
void trim(std::string &s);
std::string intToString(int i);
StringVec split(const std::string &s, char delim);
StringVec splitFirst(const std::string &s, char delim);

// File operations
std::string readFile(const std::string &path);
bool isDirectory(const std::string &path);
bool fileExists(const std::string &path);

// Containers
void printVec(const StringVec &vec);
void printMap(const StringMap &map);

#endif
