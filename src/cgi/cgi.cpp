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

void cgi_response(const std::string &message, HttpResponse *response, short code)
{
    response->setStatusCode(code);
    if(code != 200)
        response->setHeader("Content-Type", "text/plain");
    else
        response->setHeader("Content-Type", "text/html");

    response->setHeader("Content-Length", intToString(message.size()));
    response->setHeader("Connection", "close");
    response->setBody(message);
    response->build();
}

std::string close_pipes(int output_pipe[2], int input_pipe[2], bool child)
{
    
    if(child)
    dup2(output_pipe[1], STDOUT_FILENO);
    
    close(output_pipe[1]);
    std::string output = read_output(output_pipe[0]);
    close(output_pipe[0]);

    if(child)
        dup2(input_pipe[0], STDIN_FILENO);
    close(input_pipe[0]); 
    close(input_pipe[1]);
    return output;
}

void Cgi::execute(const std::string &scriptPath, HttpResponse *response, const HttpRequest &request) 
{
    Type scriptType = detect_type(scriptPath);
    std::string interpreter = get_interpreter(scriptType);

    if (scriptType == UNKNOWN)
    {
        cgi_response("Not Implemented", response, 501);
        return;
    }

    int output_pipe[2];
    int input_pipe[2];

    if (pipe(output_pipe) == -1 || pipe(input_pipe) == -1)
    {
        cgi_response("Failed to create pipes", response, 500);
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        close_pipes(output_pipe, input_pipe, false);
        cgi_response("Failed to fork process", response, 500);

        return;
    }

    if (pid == 0) // child process
    {
        close_pipes(output_pipe, input_pipe, true);
        char *argv[3] = {str_char(interpreter), str_char(scriptPath), NULL};
        std::map<std::string, std::string> envMap = get_env(scriptPath, request);
        char **env = convert_env(envMap);

        execve(str_char(interpreter), argv, env);
    }
    
    if (request.getMethod() == "POST" || request.getMethod() == "DELETE")
    {
        const std::string &body = request.getBody();
        if (!body.empty())
            write(input_pipe[1], body.c_str(), body.size());
    }

    std::string output = close_pipes(output_pipe, input_pipe, false);

    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
    {
        cgi_response("Script execution failed", response, 500);
        return;
    }

    StringMap headers = get_headers(output);
    std::string body = get_body(output);

    if (headers.empty())
    {
        cgi_response("Empty response headers", response, 500);
        return;
    }

    for (StringMap::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        response->setHeader(it->first, it->second);
    }

    if (body.empty() || body.size() <= 1)
    {
        cgi_response("Empty response body", response, 500);
        return;
    }

    cgi_response(body, response, 200);
}
