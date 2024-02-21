#include "Message.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "debug.hpp"
#include "webserv.hpp"
#include <stdexcept>

#ifndef IRequestHandler_HPP
#define IRequestHandler_HPP

using namespace webserv::http;

class IRequestHandler
{
    public:
        virtual ~IRequestHandler()
        {
            DEBUG_MSG("IRequestHandler destructor called", M);
        };
        virtual Response handle_request(const Request &request) = 0;
};

// concrete types
class GetRequestHandler : public IRequestHandler
{
    public:
        ~GetRequestHandler()
        {
            DEBUG_MSG("GetRequestHandler destructor called", M);
        };
        Response handle_request(const Request &request);

    private:
        void               add_header(pair<string, string> header_field);
        const vector<char> get_resource(const string &resource, const vsp &headers);
        // data members
        STATUS_CODE  status;
        vector<char> body;
        vsp          response_headers;
};

class RequestHandlerFactory
{
    public:
        static IRequestHandler *MakeRequestHandler(METHOD m)
        {
            switch (m)
            {
                case GET: return new GetRequestHandler;
                default: throw std::runtime_error("Request Method not implemented\n");
            }
        }
};

#endif // IRequestHandler_HPP
