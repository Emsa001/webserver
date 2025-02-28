#include "Server.hpp"

std::vector<std::string> listFiles(const std::string &directory) {
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                files.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }
    return files;
}

std::string Server::GET(const std::string &request) {
    int path_start = request.find("GET /") + 5;
    int path_end = request.find(" ", path_start);
    std::string path = request.substr(path_start, path_end - path_start);
    
    if (path == "files") 
    {
        std::vector<std::string> files = listFiles("./uploads");
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
        response += "[";
        for (size_t i = 0; i < files.size(); i++) {
            response += "\"" + files[i] + "\"";
            if (i < files.size() - 1) {
                response += ",";
            }
        }
        response += "]";
        return response;
    } 
    else if (path == "") 
    {
        std::ifstream file("./pages/index.html");
        if (!file) 
            return "HTTP/1.1 404 Not Found\r\n\r\n";
        
        std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n";
        
        std::string line;
        while (std::getline(file, line)) {
            response += line + "\n";
        }
        file.close();
        return response;
    } 
    else 
        return "HTTP/1.1 404 Not Found\r\n\r\n";
}