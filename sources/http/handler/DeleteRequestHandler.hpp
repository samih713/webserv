#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "webserv.hpp"

#ifndef DELETEREQUESTHANDLER_HPP
#define DELETEREQUESTHANDLER_HPP

class DeleteRequestHandler: public RequestHandlerBase {
public:
    DeleteRequestHandler();
    ~DeleteRequestHandler();
    Response handle_request(const Request& request, const CachedPages* cachedPages,
        const ServerConfig& config);

private:
    const vector<char> get_resource(const Request& request,
        const CachedPages* cachedPages, const ServerConfig& config);
};

#endif // DELETEREQUESTHANDLER_HPP