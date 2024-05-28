#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef GET_REQUEST_HANDLER_HPP
#define GET_REQUEST_HANDLER_HPP

class GetRequestHandler: public RequestHandlerBase {
public:
    GetRequestHandler();
    ~GetRequestHandler();
    Response handle_request(const Request& request, CachedPages& cached,
        const ServerConfig& config);

private:
    const vector<char> get_resource(const Request& request, CachedPages& cached,
        const ServerConfig& config);
};

#endif // GET_REQUEST_HANDLER_HPP
