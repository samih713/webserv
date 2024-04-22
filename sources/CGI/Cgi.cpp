#include "Cgi.hpp"

static char** headers_to_env(const vsp& headers);
static string get_uri(string res);
static string get_query_string(string res);

Cgi::Cgi(const Request& request)
{
    string resource = request.get_resource();
    headers         = request.get_headers();
    environment     = headers_to_env(headers);
    filePath        = get_uri(resource);
    queryString     = get_query_string(resource);

    arguments    = new char*[2];
    arguments[0] = const_cast<char*>(filePath.c_str());
    arguments[1] = NULL;
}

Cgi::~Cgi()
{
    for (int i = 0; environment[i] != NULL; ++i)
        delete[] environment[i];
    delete[] environment;
    delete[] arguments;
}

void Cgi::execute(const string& outputFile)
{
    int         fd[2];
    int         id;
    string res_body;

    // Create a pipe for communication
    if (pipe(fd) == -1) {
        cerr << "Error creating pipe: " << strerror(errno) << endl;
        return;
    }

    // Fork the process
    id = fork();
    if (id == -1) {
        cerr << "Error forking process: " << strerror(errno) << endl;
        return;
    }
    else if (id == 0) {
        // Child process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        if (execve(const_cast<char*>(filePath.c_str()), arguments, environment) == -1) {
            cerr << "Error executing execve: " << strerror(errno) << endl;
            _exit(EXIT_FAILURE);
        }
    }

    // Parent process
    close(fd[1]); // Close the write end of the pipe

    // Wait for the child process to finish
    int status;
    waitpid(id, &status, 0);

    // Read the response from the pipe
    char    buffer[91];
    ssize_t bytesRead;
    while ((bytesRead = read(fd[0], buffer, 90)) > 0)
        res_body.append(buffer, bytesRead);
    // Save the response to a file
    ofstream outFile(outputFile.c_str());
    if (outFile.is_open()) {
        outFile << res_body;
        outFile.close();
        cout << "Response saved to: " << outputFile << endl;
    }
    else {
        cerr << "Error opening output file: " << strerror(errno) << endl;
    }

    close(fd[0]);
}

string Cgi::execute(void)
{
    int         fd[2];
    int         id;
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

        if (execve(const_cast<char*>(filePath.c_str()), arguments, environment) == -1) {
            cerr << "Error executing execve: " << strerror(errno) << endl;
            _exit(EXIT_FAILURE);
        }
    }

    // Parent process
    close(fd[1]); // Close the write end of the pipe

    // Wait for the child process to finish
    int status;
    waitpid(id, &status, 0);

    // Read the response from the pipe
    char    buffer[91];
    ssize_t bytesRead;
    while ((bytesRead = read(fd[0], buffer, 90)) > 0)
        res_body.append(buffer, bytesRead);

    close(fd[0]);
    return (res_body);
}

static char** headers_to_env(const vsp& headers)
{
    vector<char*> envVector;

    // Iterate through headers
    vsp::const_iterator end = headers.end();
    for (vsp::const_iterator it = headers.begin(); it != end; ++it) {
        size_t len      = it->first.size() + it->second.size() + 2;
        char*  envEntry = new char[len];
        std::snprintf(envEntry, len, "%s=%s", it->first.c_str(), it->second.c_str());
        envVector.push_back(envEntry);
    }

    // Allocate memory for char* array
    char** envp = new char*[envVector.size() + 1];

    // Copy pointers from vector to char* array
    for (size_t i = 0; i < envVector.size(); ++i)
        envp[i] = envVector[i];
    envp[envVector.size()] = NULL;

    return envp;
}

static string get_uri(string res)
{
    string resn;
    size_t qu;

    qu   = res.find('?', 0);
    resn = res.substr(0, qu);
    // result = const_cast<char *>(res.substr(0,qu).c_str());
    return (resn);
}

static string get_query_string(string res)
{
    string resn;
    size_t qu;
    size_t length;

    qu     = res.find('?', 0);
    length = res.length();
    resn   = res.substr(qu + 1, length);
    return (resn);
}
