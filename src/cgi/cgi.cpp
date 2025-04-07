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

static std::string build_query_string(const StringMap &query)
{
    std::string queryString;
    for (StringMap::const_iterator it = query.begin(); it != query.end(); ++it)
    {
        if (!queryString.empty())
            queryString += "&";
        queryString += it->first + "=" + it->second;
    }
    return queryString;
}

void Cgi::execute(const std::string &scriptPath, HttpResponse *response, const HttpRequest &request) 
{
    std::string query = build_query_string(request.getURL()->getQueryMap());
    std::cout << "Query: " << query << std::endl;

    Type scriptType = detect_type(scriptPath);
    std::string interpreter = get_interpreter(scriptType);

    if (scriptType == UNKNOWN)
    {
        cgi_response("Not Implemented", response, 501);
        return;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        cgi_response("Failed to create pipe", response, 500);
        return;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        cgi_response("Failed to fork process", response, 500);
        return;
    }

    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        char *argv[3] = {str_char(interpreter), str_char(scriptPath), NULL};
        std::map<std::string, std::string> envMap = get_env(scriptPath, query);
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
        cgi_response("Script execution failed", response, 500);
        return;
    }

    StringMap headers = get_headers(output);
    if (headers.empty())
    {
        cgi_response("Empty response headers", response, 500);
        return;
    }
    for (StringMap::iterator it = headers.begin(); it != headers.end(); ++it)
    {
        response->setHeader(it->first, it->second);
    }
    std::string body = get_body(output);
    if (body.empty() || body.size() <= 1)
    {
        cgi_response("Empty response body", response, 500);
        return;
    }

    cgi_response(body, response, 200);
}