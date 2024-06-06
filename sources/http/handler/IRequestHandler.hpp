#include "CachedPages.hpp"
#include "Response.hpp"
#include "ServerConfig.hpp"
#include "Logger.hpp"

#ifndef IREQUEST_HANDLER_HPP
#define IREQUEST_HANDLER_HPP

// interface for request handlers
class IRequestHandler {
public:
    virtual ~IRequestHandler() { LOG_DEBUG("IRequestHandler: destructor called"); };
    virtual Response handle_request(const Request& request) = 0;
};

#endif // IREQUEST_HANDLER_HPP
