#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef DELETE_REQUEST_HANDLER_HPP
#define DELETE_REQUEST_HANDLER_HPP

class DeleteRequestHandler: public RequestHandlerBase {
public:
    DeleteRequestHandler(const ServerConfig& cfg, CachedPages& cp)
        : RequestHandlerBase(cfg, cp)
    {
        DEBUG_MSG("DeleteRequestHandler constructor called", B);
    }
    ~DeleteRequestHandler() { DEBUG_MSG("DeleteRequestHandler destructor called", B); };

    Response handle_request(const Request& r);

private:
    const vector<char> get_resource(const Request& r);
};

#endif // DELETE_REQUEST_HANDLER_HPP
