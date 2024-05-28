#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef DELETE_REQUEST_HANDLER_HPP
#define DELETE_REQUEST_HANDLER_HPP

class DeleteRequestHandler: public RequestHandlerBase {
public:
    DeleteRequestHandler();
    ~DeleteRequestHandler();
    Response handle_request(const Request& request, CachedPages& cached,
        const ServerConfig& config);

private:
    const vector<char> get_resource(const Request& request, CachedPages& cached,
        const ServerConfig& config);
};

#endif // DELETE_REQUEST_HANDLER_HPP
