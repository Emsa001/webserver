#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

// String operations
void trim(std::string &s);
std::string int_to_string(int i);


// File operations
std::string read_file(const std::string &path);

#endif