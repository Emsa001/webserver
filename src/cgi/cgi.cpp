#include "Cgi.hpp"

static char *str_char(const std::string &str)
{
    return const_cast<char *>(str.c_str());
}

std::string Cgi::execute(const std::string &scriptPath)
{
    Type scriptType = detect_type(scriptPath);
    std::string interpreter = get_interpreter(scriptType);

    if (scriptType == UNKNOWN)
        return "HTTP/1.1 415 Unsupported Media Type\n\nUnsupported script type.";

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

        char *argv[3] = {str_char(interpreter), str_char(scriptPath), NULL};
        std::map<std::string, std::string> envMap = get_env(scriptPath);
        char **env = convert_env(envMap);

        execve(str_char(interpreter), argv, env);
        exit(1);
    }

    close(pipe_fd[1]);

    char buffer[1024];
    std::stringstream response;
    ssize_t bytesRead;
    while ((bytesRead = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        response << buffer;
    }
    close(pipe_fd[0]);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        return "HTTP/1.1 500 Internal Server Error\n\nCGI script execution failed.";

    return "HTTP/1.1 200 OK\n" + response.str();
}