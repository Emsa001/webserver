#include "webserv.hpp"

std::string cgi_execute(const std::string &scriptPath) 
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
        return "HTTP/1.1 500 Internal Server Error\n\nFailed to create pipe.";

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
    while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0) 
    {
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
