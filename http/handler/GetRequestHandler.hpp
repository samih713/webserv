#include "IRequestHandler.hpp"
#include "webserv.hpp"

#ifndef GETREQUESTHANDLER_HPP
#define GETREQUESTHANDLER_HPP

using namespace webserv::http;

class GetRequestHandler : public IRequestHandler
{
    public:
        GetRequestHandler();
        ~GetRequestHandler();
        Response handle_request(const Request &request);

        ifstream     _not_found;
        vector<char> not_found;

    private:
        const vector<char> get_resource(string resource, const vsp &headers);
};

#endif // GETREQUESTHANDLER_HPP
