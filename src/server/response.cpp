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

std::string GET(const std::string &request) {
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

bool saveFile(const std::string &filePath, const std::string &fileData) {

    std::ofstream file(filePath.c_str(), std::ios::binary);
    if (!file)
        return false;

    file.write(fileData.c_str(), fileData.size());
    file.close();
    return true;
}

void POST(const std::string &request) {
    size_t num = request.find("Content-Length: ");
    if (num == std::string::npos) {
        return;
    }
    std::string filePath = "./uploads/"; //
    size_t start = request.find("filename=\"") + 10;
    size_t end = request.find("\"", start);
    std::string name = request.substr(start, end - start);
    filePath += name;

    size_t data_start = request.find("\r\n\r\n") + 4;
    std::string fileData = request.substr(data_start);

    saveFile(filePath, fileData);
}

bool deleteFile(const std::string &filePath) {
    return (remove(filePath.c_str()) == 0);
}

std::string DELETE(const std::string &request) {
    int path_start = request.find("DELETE /") + 8;
    int path_end = request.find(" ", path_start);
    std::string path = request.substr(path_start, path_end - path_start);

    std::string filePath = "./uploads/" + path;
    if (deleteFile(filePath))
        return "HTTP/1.1 200 OK\r\n\r\nFile deleted successfully.";
    else
        return "HTTP/1.1 404 Not Found\r\n\r\nFile not found.";
}

std::string Server::get_response(Client &client) {
    std::string const &request = client.get_request();

    if (request.find("GET /") == 0) {
        return GET(request);
    } else if (request.find("POST /upload") == 0) {
        POST(request);
        return "HTTP/1.1 200 OK\r\n\r\nFile uploaded successfully.";
    } else if (request.find("DELETE /") == 0) {
        return DELETE(request);
    } else {
        return "HTTP/1.1 400 Bad Request\r\n\r\n";
    }
}