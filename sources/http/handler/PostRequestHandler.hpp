#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef POST_REQUEST_HANDLER_HPP
#define POST_REQUEST_HANDLER_HPP

class PostRequestHandler: public RequestHandlerBase {
public:
    PostRequestHandler(ServerConfig& cfg) : RequestHandlerBase(cfg)
    {}
    ~PostRequestHandler() {}

    Response handle_request(const Request& r);
};

#endif // POST_REQUEST_HANDLER_HPP
