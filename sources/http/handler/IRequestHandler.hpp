#include "../../server/Server.hpp"
#include "../response/Response.hpp"

#ifndef IRequestHandler_HPP
#define IRequestHandler_HPP

// interface
class IRequestHandler {
public:
    virtual ~IRequestHandler() { DEBUG_MSG("IRequestHandler destructor called", M); };
    virtual Response handle_request(const Request& request,
        const CachedPages* cachedPages, const ServerConfig& config) = 0;
};

#endif // IRequestHandler_HPP
