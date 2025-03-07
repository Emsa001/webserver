#include "Server.hpp"

bool saveFile(const std::string &filePath, const std::string &fileData) {

    std::ofstream file(filePath.c_str(), std::ios::binary);
    if (!file)
        return false;

    file.write(fileData.c_str(), fileData.size());
    file.close();
    return true;
}

std::string Server::POST(const std::string &request) 
{
    int path_start = request.find("POST /") + 6;
    int path_end = request.find(" ", path_start);
    std::string path = request.substr(path_start, path_end - path_start);
    
    if (path == "upload") 
    {
        size_t num = request.find("Content-Length: ");
        if (num == std::string::npos)
            return "HTTP/1.1 400 Bad Request\r\n\r\n";
    
        std::string filePath = "./uploads/"; //
        size_t start = request.find("filename=\"") + 10;
        size_t end = request.find("\"", start);
        std::string name = request.substr(start, end - start);
        filePath += name;
    
        size_t data_start = request.find("\r\n\r\n") + 4;
        std::string fileData = request.substr(data_start);
    
        if(saveFile(filePath, fileData))
            return "HTTP/1.1 200 OK\r\n\r\nFile uploaded successfully.";
        else
            return "HTTP/1.1 500 Internal Server Error\r\n\r\n";
    }
    return "HTTP/1.1 404 Not Found\r\n\r\n";
}
