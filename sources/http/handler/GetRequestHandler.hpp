#include "../../../includes/webserv.hpp"
#include "../../server/Server.hpp"
#include "../request/Request.hpp"
#include "../response/Response.hpp"
#include "./RequestHandlerBase.hpp"

#ifndef GETREQUESTHANDLER_HPP
#define GETREQUESTHANDLER_HPP

class GetRequestHandler: public RequestHandlerBase {
public:
    GetRequestHandler();
    ~GetRequestHandler();
    Response handle_request(const Request& request, const CachedPages* cachedPages,
        const ServerConfig& config);

private:
    const vector<char> get_resource(const Request& request, const CachedPages* cachedPages,
        const ServerConfig& config);
};

#endif // GETREQUESTHANDLER_HPP
