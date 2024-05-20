#ifndef CGI_HPP
#define CGI_HPP

#include "Request.hpp"
#include "webserv.hpp"
#include "Request.hpp"
#include "Server.hpp"

class CGI {
public:
    CGI(const Request& request, const ServerConfig& config);
    ~CGI();

    string execute(void);
    char** headers_to_env(const Request& request, const ServerConfig& config);


private:
    HeaderMap headers;
    string  _queryString;
    char*   _filePath;
    string  _body;
    char**  _arguments;
    char**  _environment;
};

#endif
