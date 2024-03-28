#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "webserv.hpp"

#ifndef GETREQUESTHANDLER_HPP
#define GETREQUESTHANDLER_HPP

using namespace webserv::http;

class GetRequestHandler : public RequestHandlerBase
{
    public:
        GetRequestHandler();
        ~GetRequestHandler();
        Response handle_request(const Request     &request,
                                const CachedPages *cachedPages,
                                const Config      &config);

    private:
        const vector<char> get_resource(const Request     &request,
                                        const CachedPages *cached,
                                        const Config      &config);
};

#endif // GETREQUESTHANDLER_HPP
