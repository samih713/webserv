#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef GET_REQUEST_HANDLER_HPP
#define GET_REQUEST_HANDLER_HPP

class GetRequestHandler: public RequestHandlerBase {
public:
    GetRequestHandler(ServerConfig& cfg, CachedPages& cp) : RequestHandlerBase(cfg, cp) {}
    ~GetRequestHandler() {}

    Response handle_request(const Request& r);

private:
    const vector<char> get_resource(const Request& r);
};

#endif // GET_REQUEST_HANDLER_HPP
