#include "../request/Request.hpp"
#include "./RequestHandlerBase.hpp"
#include "../response/Response.hpp"
#include "../../server/Server.hpp"
#include "../../../includes/webserv.hpp"

#ifndef GETREQUESTHANDLER_HPP
#define GETREQUESTHANDLER_HPP

class GetRequestHandler : public RequestHandlerBase
{
    public:
        GetRequestHandler();
        ~GetRequestHandler();
        Response handle_request(const Request     &request,
                                const CachedPages *cachedPages);

    private:
        const vector<char> get_resource(const Request     &request,
                                        const CachedPages *cached);
};

#endif // GETREQUESTHANDLER_HPP
