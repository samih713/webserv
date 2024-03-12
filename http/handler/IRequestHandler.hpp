#include "Response.hpp"
#include "webserv.hpp"

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
        virtual Response handle_request(const Request &request) = 0;
        virtual void     add_header(pair<string, string> header_field) = 0;
};


#endif // IRequestHandler_HPP
