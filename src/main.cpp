#include "../includes/webserv.hpp"

std::string executeCGI(const std::string &scriptPath) 
{
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

        char *argv[] = {const_cast<char *>(scriptPath.c_str()), NULL};
        char *envp[] = {NULL};
        execve(scriptPath.c_str(), argv, envp);
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
        return executeCGI("/cgi/hello.py");

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<!DOCTYPE html><html><head><title>Hello, World!</title></head>"
        "<body><h1>Hello, World!</h1></body></html>";
    return response;
}


int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) 
        return 1;

    // Set SO_REUSEADDR option
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Setsockopt failed: " << strerror(errno) << std::endl;
        close(server_fd);
        return 1;
    }

    sockaddr_in server_addr;
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
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
        if (client_fd == -1)
            continue;

        char buffer[1024] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);
        std::string html = response(buffer);

        send(client_fd, html.c_str(), html.size(), 0);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}