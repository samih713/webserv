#include "IRequestHandler.hpp"
#include "Server.hpp"
#include "webserv.hpp"

#ifndef GETREQUESTHANDLER_HPP
#define GETREQUESTHANDLER_HPP

using namespace webserv::http;

class GetRequestHandler : public IRequestHandler
{
    public:
        GetRequestHandler();
        ~GetRequestHandler();
        Response handle_request(const Request     &request,
                                const CachedPages &cachedPages,
                                const Config      &config);
        void     add_header(pair<string, string> header_field)
        {
            response_headers.push_back(header_field);
        }

    private:
        const vector<char> get_resource(const Request     &request,
                                        const CachedPages &cached,
                                        const Config      &config);
        STATUS_CODE        status;
        vector<char>       body;
        vsp                response_headers;
};

#endif // GETREQUESTHANDLER_HPP
