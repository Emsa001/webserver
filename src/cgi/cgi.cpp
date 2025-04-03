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

void server_error(const std::string &message, HttpResponse *response)
{
    response->setStatusCode(500);
    response->setHeader("Content-Type", "text/plain");
    response->setBody(message);
    response->build();
}

// headers in cgi response
// echo "Content-Type: text/html"
// echo "test: test"
// echo "test2: test2"
// echo ""

// body in cgi response
// echo "<html><body><h1>Shell!</h1></body></html>"

std::string get_body(const std::string &output)
{
    std::string body;
    std::istringstream stream(output);
    std::string line;
    bool isBody = false;

    while (std::getline(stream, line))
    {
        if (line.empty())
        {
            isBody = true;
            body += "\n";
            continue;
        }
        if (isBody)
        {
            body += line + "\n";
        }
    }
    return body;
}

StringMap get_headers(const std::string &output)
{
    StringMap headers;
    std::istringstream stream(output);
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.empty())
            break;

        size_t pos = line.find(':');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            headers[key] = value;
        }
    }
    return headers;
}

void Cgi::execute(const std::string &scriptPath, HttpResponse *response)
{
    Type scriptType = detect_type(scriptPath);
    std::string interpreter = get_interpreter(scriptType);

    if (scriptType == UNKNOWN)
    {
        server_error("Unknown script type", response);
        return;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        server_error("Failed to create pipe", response);
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        server_error("Failed to fork process", response);
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
    std::string output = read_output(pipe_fd[0]);
    close(pipe_fd[0]);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        server_error("Script execution failed", response);
        return;
    }

    std::string body = get_body(output);
    if (body.empty())
    {
        server_error("Empty response body", response);
        return;
    }
    std::cout << "Body: (" << body << ")" << std::endl;
    StringMap headers = get_headers(output);
    for (StringMap::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        response->setHeader(it->first, it->second);
    }

    response->setStatusCode(200);
    response->setHeader("Content-Type", "text/html");
    response->setHeader("Content-Length", intToString(output.size()));
    response->setBody(body);
    response->build();
}