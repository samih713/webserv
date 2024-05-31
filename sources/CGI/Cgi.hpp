#include "Request.hpp"
#include "Server.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef CGI_HPP
#define CGI_HPP

class CGI {
public:
    CGI(const Request& request, const ServerConfig& cfg, CachedPages& cp);
    ~CGI();

    vector<char> execute(void);

private:
    string  _queryString;
    string  _filePath;
    string  _body;
    char**  _arguments;
    char**  _environment;
    TimeOut _timer;

    CachedPages& _cp;

    char** set_environment(const Request& request, const ServerConfig& cfg);
};

#endif
