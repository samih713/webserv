#include "Response.hpp"
#include "Server.hpp"

#ifndef IRequestHandler_HPP
#define IRequestHandler_HPP

using namespace webserv::http;

// interface
class IRequestHandler
{
    public:
        virtual ~IRequestHandler()
        {
            DEBUG_MSG("IRequestHandler destructor called", M);
        };
        virtual Response handle_request(const Request     &request,
                                        const CachedPages *cachedPages,
                                        const Config      &config) = 0;
};

#endif // IRequestHandler_HPP
