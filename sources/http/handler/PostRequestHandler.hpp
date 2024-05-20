#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "webserv.hpp"

#ifndef POSTREQUESTHANDLER_HPP
#define POSTREQUESTHANDLER_HPP

class PostRequestHandler: public RequestHandlerBase {
public:
    PostRequestHandler();
    ~PostRequestHandler();
    Response handle_post_request(const Request& request, const CachedPages* cachedPages,
        const ServerConfig& config);

private:
};

#endif // POSTREQUESTHANDLER_HPP
