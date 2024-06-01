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

    vector<char> execute(int& cgiStatus, fd& cgiReadFd, pid_t& cgiChild);

private:
    string  _queryString;
    string  _filePath;
    string  _body;
    char**  _arguments;
    char**  _environment;
    TimeOut _timer; //! need more testing

    CachedPages& _cp;

    pid_t  execute_child(fd& cgiReadFd);
    char** set_environment(const Request& request, const ServerConfig& cfg);
};

#endif
