#include "Request.hpp"
#include "Server.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef CGI_HPP
#define CGI_HPP

class CGI {
public:
    CGI(const Request& request, const ServerConfig& cfg);
    ~CGI();

    string execute(void);
    char** headers_to_env(const Request& request, const ServerConfig& cfg);

private:
    string  _queryString;
    string  _filePath;
    string  _body;
    char**  _arguments;
    char**  _environment;
    TimeOut _timer;
};

#endif
