#include "Cgi.hpp"

static char *str_char(const std::string &str)
{
    return const_cast<char *>(str.c_str());
}


std::string read_output(int pipe_fd)
{
    char buffer[1024];
    std::stringstream response;
    ssize_t bytesRead;
    while ((bytesRead = read(pipe_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        response << buffer;
    }
    return response.str();
}

void Cgi::execute(const std::string &scriptPath, HttpResponse *response)
{
    Type scriptType = detect_type(scriptPath);
    std::string interpreter = get_interpreter(scriptType);

    if (scriptType == UNKNOWN)
    {
        response->setStatusCode(500);
        response->setHeader("Content-Type", "text/plain");
        response->setResponse("Unsupported script type");
        response->build();
        return;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        response->setStatusCode(500);
        response->setHeader("Content-Type", "text/plain");
        response->setResponse("Failed to create pipe");
        response->build();
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        response->setStatusCode(500);
        response->setHeader("Content-Type", "text/plain");
        response->setResponse("Failed to fork process");
        response->build();
        return;
    }

    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        char *argv[3] = {str_char(interpreter), str_char(scriptPath), NULL};
        std::map<std::string, std::string> envMap = get_env(scriptPath);
        char **env = convert_env(envMap);

        execve(str_char(interpreter), argv, env);
    }

    close(pipe_fd[1]);
    std::string body = read_output(pipe_fd[0]);
    close(pipe_fd[0]);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        response->setStatusCode(500);
        response->setHeader("Content-Type", "text/plain");
        response->setResponse("CGI script execution failed");
        response->build();
        return;
    }


    FileData fileData;
    fileData.exists = true;
    fileData.isDirectory = false;

    response->setStatusCode(200);
    response->setHeader("Content-Type", "text/html");
    response->setHeader("Content-Length", intToString(body.size()));
    response->setBody(body);
    response->build();
}