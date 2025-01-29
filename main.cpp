#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define PORT 8080

std::string executeCGI(const std::string &scriptPath) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        return "HTTP/1.1 500 Internal Server Error\n\nFailed to create pipe.";
    }

    pid_t pid = fork();
    if (pid < 0)
        return "HTTP/1.1 500 Internal Server Error\n\nFork failed.";

    if (pid == 0) 
    {
        close(pipe_fd[0]);

        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        // Execute the CGI script
        char *argv[] = {const_cast<char *>(scriptPath.c_str()), NULL};
        char *envp[] = {NULL};
        execve(scriptPath.c_str(), argv, envp);
        exit(1);
    }

    close(pipe_fd[1]);

    // Read the CGI script output from the pipe
    char buffer[1024];
    std::stringstream ss;
    ssize_t bytesRead;
    while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
    }
    close(pipe_fd[0]);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        return "HTTP/1.1 500 Internal Server Error\n\nCGI script execution failed.";

    return "HTTP/1.1 200 OK\n" + ss.str();
}

std::string response(const std::string &request) 
{
    // Check if the request targets the CGI script
    if (request.find("GET /cgi-bin/hello.py") == 0)
        return executeCGI("./hello.py");

    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\n";
    ss << "Content-Type: text/html\n";
    ss << "Content-Length: 13\n";
    ss << "Connection: close\n";
    ss << "\n";
    ss << "Hello, World!";
    return ss.str();
}

int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) 
        return 1;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1 || listen(server_fd, 5) == -1) 
    {
        std::cerr << "Bind or listen failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }

    std::cout << "Server is running on http://localhost:" << PORT << std::endl;

    while (true) 
    {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {
            std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            continue;
        }

        // Read the client's request
        char buffer[1024] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);

        // Generate the appropriate response
        std::string html = response(buffer);
        send(client_fd, html.c_str(), html.size(), 0);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
