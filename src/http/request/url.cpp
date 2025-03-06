#include "HttpRequest.hpp"

HttpURL::HttpURL(const std::string &url) {
    StringVec parts = splitFirst(url, '?');

    if(parts.size() == 0){
        std::cerr << "Invalid URL" << std::endl;
        return ;
    }

    this->path = parts[0];
    if(parts.size() == 1){
        this->query = "";
        return ;
    }

    this->query = parts[1];
    parts = split(this->query, '&');

    StringVec::const_iterator it = parts.begin();
    for(; it != parts.end(); it++){
        StringVec pair = splitFirst(*it, '=');
        if(pair.size() != 2){
            this->queryMap[pair[0]] = "";
            continue;
        }
        this->queryMap[pair[0]] = pair[1];
    }
}