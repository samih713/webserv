#include "Cgi.hpp"

CGI::CGI(const Request& request, const ServerConfig& cfg)
    : _body(request.get_body()), _timer(request.timer)
{
    string resource = request.get_resource();
    _filePath       = resource;
    if (!request.get_query_string().empty())
        _queryString = request.get_query_string();
    _environment = headers_to_env(request, cfg);
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

char** CGI::headers_to_env(const Request& request, const ServerConfig& cfg)
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

    // "PATH_TRANSLATED" "REMOTEaddr" "REMOTE_IDENT" "REMOTE_USER"

    // https://example.com/path/to/page?name=ferret&color=purple
    // QUERY_STRING=name=ferret color=purple

	// ! Query string is stored already in request
    string envQueryString = "QUERY_STRING=";
    char*  token          = strtok(const_cast<char*>(_queryString.c_str()), "&");
    // Iterate through the tokens and push integers onto the stack
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

string CGI::execute(void)
{
    int    fd[2];
    int    id;
    string res_body;

    // Create a pipe for communication
    if (pipe(fd) == -1) {
        cerr << "Error creating pipe: " << strerror(errno) << endl;
        return NULL;
    }

    // Fork the process
    // TODO fix forking here for sleep
    id = fork();
    if (id == -1) {
        cerr << "Error forking process: " << strerror(errno) << endl;
        return NULL;
    }
    else if (id == 0) {
        // Child process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (execve(_arguments[0], _arguments, _environment) == -1) {
            cerr << "Error executing execve: " << strerror(errno) << endl;
            _exit(EXIT_FAILURE);
        }
    }

    close(fd[1]); // Close the write end of the pipe

    // Parent process
    int childStatus;
    // Wait for the child process to finish or until timeout
    while (true) {
        // Check if the timeout duration has been reached
        if (_timer.is_timeout()) {
            cerr << "Timeout reached. Exiting." << endl;
            // Terminate the child process
            kill(id, SIGTERM);
            break;
        }

        // Check if the child process has finished
        if (waitpid(id, &childStatus, WNOHANG) == id) {
            // Child process has finished
            // Read from the pipe
            char    buffer[1024]; //!
            ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer));
            if (bytesRead > 0) {
                res_body.append(buffer, bytesRead);
            }
            else if (bytesRead == 0) {
                break;
            }
            else {
                // Error while reading from the pipe
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // No data available yet, continue waiting
                    usleep(100000);
                    continue;
                }
                else {
                    cerr << "Error reading from pipe: " << strerror(errno) << endl;
                    break;
                }
            }
            break;
        }
        usleep(100000);
    }

    close(fd[0]);
    return res_body;
}
