#include "Response.hpp"
#include "Server.hpp"

#ifndef IREQUESTHANDLER_HPP
#define IREQUESTHANDLER_HPP

// interface for request handlers
class IRequestHandler {
public:
    virtual ~IRequestHandler() { DEBUG_MSG("IRequestHandler destructor called", M); };
    virtual Response handle_request(const Request& request,
        const CachedPages* cachedPages, const ServerConfig& config) = 0;
};

#endif // IREQUESTHANDLER_HPP
