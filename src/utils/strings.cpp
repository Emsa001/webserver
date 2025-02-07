#include "webserv.hpp"


// Function object for trimming from start
struct IsNotSpace {
    bool operator()(unsigned char ch) const {
        return !std::isspace(ch);
    }
};


void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), IsNotSpace()));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), IsNotSpace()).base(), s.end());
}


void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::string getFirstWord(std::string const s) {
    if (s.empty())
        return "";
    std::string::size_type pos = s.find(' ');
    if (pos != std::string::npos) {
        return s.substr(0, pos);
    }
    return s;
}