#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "webserv.hpp"

#ifndef POST_REQUEST_HANDLER_HPP
#define POST_REQUEST_HANDLER_HPP

class PostRequestHandler: public RequestHandlerBase {
public:
    PostRequestHandler();
    ~PostRequestHandler();
    Response handle_request(const Request& request, const CachedPages* cachedPages,
        const ServerConfig& config);
};

#endif // POST_REQUEST_HANDLER_HPP
