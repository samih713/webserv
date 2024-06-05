#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef GET_REQUEST_HANDLER_HPP
#define GET_REQUEST_HANDLER_HPP

class GetRequestHandler: public RequestHandlerBase {
public:
    GetRequestHandler(ServerConfig& cfg, CachedPages& cp) : RequestHandlerBase(cfg, cp)
    {
        DEBUG_MSG("GetRequestHandler constructor called", B);
    }
    ~GetRequestHandler() { DEBUG_MSG("GetRequestHandler destructor called", B); }

    Response handle_request(const Request& r);

private:
    const vector<char> get_resource(const Request& r);
    vector<char>       list_directory(const string& dirPath, const string& uri);
};

#endif // GET_REQUEST_HANDLER_HPP
