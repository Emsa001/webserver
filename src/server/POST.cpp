#include "Server.hpp"

bool saveFile(const std::string &filePath, const std::string &fileData) {
    std::ofstream file(filePath.c_str(), std::ios::binary | std::ios::app); // Append mode for large files
    if (!file) return false;

    file.write(fileData.c_str(), fileData.size());
    file.close();
    return true;
}

std::string Server::POST(const std::string &request) {
    size_t contentLengthPos = request.find("Content-Length: ");
    if (contentLengthPos == std::string::npos) {
        return "HTTP/1.1 400 Bad Request\r\n\r\nMissing Content-Length";
    }

    // **Extract the boundary from the Content-Type header**
    size_t boundaryPos = request.find("Content-Type: multipart/form-data; boundary=");
    if (boundaryPos == std::string::npos) {
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid Boundary";
    }

    // Move past "Content-Type: multipart/form-data; boundary="
    boundaryPos += 44;  // Length of "Content-Type: multipart/form-data; boundary="

    size_t endBoundary = request.find("\r\n", boundaryPos);
    if (endBoundary == std::string::npos) {
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid Boundary End";
    }

    std::string boundary = "--" + request.substr(boundaryPos, endBoundary - boundaryPos);

    // **Find Start of File Data**
    size_t dataStart = request.find(boundary);
    if (dataStart == std::string::npos) {
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid Boundary";
    }

    // **Extract the filename**
    size_t filenamePos = request.find("filename=\"", dataStart);
    if (filenamePos == std::string::npos) {
        return "HTTP/1.1 400 Bad Request\r\n\r\nFilename missing";
    }

    filenamePos += 10; // Move past 'filename="'
    size_t filenameEnd = request.find("\"", filenamePos);
    std::string fileName = request.substr(filenamePos, filenameEnd - filenamePos);
    std::string filePath = "./uploads/" + fileName;

    // **Find Start of File Content**
    size_t fileDataStart = request.find("\r\n\r\n", filenameEnd);
    if (fileDataStart == std::string::npos) {
        return "HTTP/1.1 400 Bad Request\r\n\r\nInvalid Data";
    }
    fileDataStart += 4; // Move past "\r\n\r\n"

    // **Find End of File Data**
    size_t fileDataEnd = request.find(boundary, fileDataStart);
    if (fileDataEnd == std::string::npos) {
        std::cout << "here1.6" << std::endl;
        return "HTTP/1.1 400 Bad Request\r\n\r\nFile Data Corrupt";
    }

    std::string fileData = request.substr(fileDataStart, fileDataEnd - fileDataStart - 2); // Remove trailing "\r\n"

    if (saveFile(filePath, fileData)) {
        return "HTTP/1.1 200 OK\r\n\r\nFile uploaded successfully.";
    } else {
        return "HTTP/1.1 500 Internal Server Error\r\n\r\nFailed to save file.";
    }
}
