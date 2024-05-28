#include "CachedPages.hpp"
#include "Response.hpp"
#include "ServerConfig.hpp"

#ifndef IREQUEST_HANDLER_HPP
#define IREQUEST_HANDLER_HPP

// interface for request handlers
class IRequestHandler {
public:
    virtual ~IRequestHandler() { DEBUG_MSG("IRequestHandler destructor called", M); };
    virtual Response handle_request(const Request& request, CachedPages& cached,
        const ServerConfig& config) = 0;
};

#endif // IREQUEST_HANDLER_HPP
