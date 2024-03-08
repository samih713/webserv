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
        void             add_header(pair<string, string> header_field)
        {
            response_headers.push_back(header_field);
        }

        STATUS_CODE  status;
        vector<char> body;
        vsp          response_headers;
};


#endif // IRequestHandler_HPP
