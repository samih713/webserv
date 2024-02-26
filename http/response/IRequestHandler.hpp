#include "FindHeaderKey.hpp"
#include "Message.hpp"
#include "Request.hpp"
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
        const vector<char> get_resource(const string &resource, const vsp &headers);
        // data members
};

// factory
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
