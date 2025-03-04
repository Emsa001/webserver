#include "Webserv.hpp"


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

std::string int_to_string(int i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

// Helper function to convert enum to string
std::string type_to_string(ValueType type) {
    switch (type) {
        case STRING: return "STRING";
        case INT: return "INT";
        case BOOL: return "BOOL";
        default: return "UNKNOWN";
    }
}