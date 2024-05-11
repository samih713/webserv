#ifndef CGI_HPP
#define CGI_HPP

#include "Request.hpp"
#include "Server.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

class Cgi {
public:
    Cgi(const Request& request, const ServerConfig& config);
    ~Cgi();
    void    execute(const string& outputFile);
    char**  headersToEnv(const Request& request, const string res,
         const ServerConfig& config);
    TimeOut timer;
    string  execute(void);

private:
    vsp    headers;
    string queryString;
    string filePath;
    string body;
    char** arguments;
    char** environment;
};

#endif
