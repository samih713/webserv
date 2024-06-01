#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef POST_REQUEST_HANDLER_HPP
#define POST_REQUEST_HANDLER_HPP

class PostRequestHandler: public RequestHandlerBase {
public:
    PostRequestHandler(ServerConfig& cfg, CachedPages& cp) : RequestHandlerBase(cfg, cp)
    {
        DEBUG_MSG("PostRequestHandler constructor called", B);
    }
    ~PostRequestHandler() { DEBUG_MSG("PostRequestHandler destructor called", B); }

    Response handle_request(const Request& r);

private:
    vector<char> process_data(const Request& r);
};

#endif // POST_REQUEST_HANDLER_HPP
