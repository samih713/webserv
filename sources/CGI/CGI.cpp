#include "CGI.hpp"

CGI::CGI(const Request& request, const ServerConfig& cfg, CachedPages& cp)
    : _requestBody(request.get_body()), _cp(cp)
{
    string resourcePath = request.get_resource_path();
    _filePath           = resourcePath;
    if (!request.get_query_string().empty())
        _queryString = request.get_query_string();
    _environment  = set_environment(request, cfg);
    _arguments    = new char*[2];
    _arguments[0] = strdup(_filePath.c_str());
    _arguments[1] = NULL;
}

CGI::~CGI()
{
    for (int i = 0; _environment[i] != NULL; ++i)
        delete[] _environment[i];
    delete[] _environment;
    for (int i = 0; _arguments[i] != NULL; ++i)
        delete[] _arguments[i];
    delete[] _arguments;
}

char** CGI::set_environment(const Request& request, const ServerConfig& cfg)
{
    vector<string> envStrings;
    HeaderMap      headers = request.get_headers();

    // Iterate through headers
    for (HeaderMap::iterator header = headers.begin(); header != headers.end(); ++header)
    {
        string envEntry = header->first + "=" + header->second;
        envStrings.push_back(envEntry);
    }

    envStrings.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envStrings.push_back("SERVER_NAME=" + cfg.serverName);
    envStrings.push_back("SERVER_SOFTWARE=" + cfg.serverName + "/1.0");
    envStrings.push_back("SERVER_PORT=" + ws_itoa(cfg.port));
    envStrings.push_back("REQUEST_URI=" + _filePath);
    envStrings.push_back("PATH_INFO=" + cfg.locations.at("/cgi-bin").root);
    envStrings.push_back("REQUEST_METHOD=" + request.get_method());

    char* token = strtok(const_cast<char*>(_queryString.c_str()), "&");
    while (token != NULL) {
        envStrings.push_back(token);
        token = strtok(NULL, "&");
    }

    // Allocate memory for char* array
    char** envp = new char*[envStrings.size() + 1];

    // Copy pointers from vector to char* array
    for (size_t i = 0; i < envStrings.size(); ++i)
        envp[i] = strdup(envStrings[i].c_str());
    envp[envStrings.size()] = NULL;

    return envp;
}

pid_t CGI::execute_child(fd& cgiReadFd)
{
    int pipeFD[2];
    int pipeIn[2];

    if (pipe(pipeFD) || pipe(pipeIn)) {
        LOG_ERROR("CGI: Error creating pipe: " + string(strerror(errno)));
        return -1;
    }

    write(pipeIn[1], _requestBody.c_str(), _requestBody.length());

    pid_t cgiChild = fork();
    if (cgiChild == -1) {
        LOG_ERROR("CGI: Error forking: " + string(strerror(errno)));
        return -1;
    }

    if (cgiChild == 0) {
        dup2(pipeFD[1], STDOUT_FILENO);
        dup2(pipeIn[0], STDIN_FILENO);
        close(pipeIn[0]);
        close(pipeIn[1]);
        close(pipeFD[0]);
        close(pipeFD[1]);

        if (execve(_arguments[0], _arguments, _environment) == -1) {
            LOG_ERROR("CGI: Error executing execve: " + string(strerror(errno)));
            _exit(EXIT_FAILURE);
        }
    }

    close(pipeIn[0]);
    close(pipeIn[1]);
    cgiReadFd = pipeFD[0];
    close(pipeFD[1]);
    return cgiChild;
}

vector<char> CGI::execute(int& cgiStatus, fd& cgiReadFd, pid_t& cgiChild)
{
    if (cgiStatus == NOT_SET) {
        // LOG_DEBUG("here");
        cgiStatus = IN_PROCESS;
        cgiChild  = execute_child(cgiReadFd);
        if (cgiChild == -1) {
            cgiStatus = COMPLETED;
            return (_cp.get_error_page(INTERNAL_SERVER_ERROR).data);
        }
    }

    vector<char> body;

    if (waitpid(cgiChild, NULL, WNOHANG) == cgiChild) {
        char buffer[1024] = { 0 };
        while (true) {
            ssize_t bytesRead = read(cgiReadFd, buffer, sizeof(buffer));
            if (bytesRead == 0)
                break;
            DEBUGASSERT(bytesRead > 0);
            if (bytesRead > 0)
                body.insert(body.end(), buffer, buffer + bytesRead);
            if (bytesRead == -1) {
                LOG_ERROR("CGI: Error reading from pipe: " + string(strerror(errno)));
                return (_cp.get_error_page(INTERNAL_SERVER_ERROR).data);
            }
        }
        cgiStatus = COMPLETED;
    }
    return body;
}
