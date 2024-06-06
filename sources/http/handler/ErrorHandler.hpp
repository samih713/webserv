#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

class ErrorHandler: public RequestHandlerBase {
public:
    ErrorHandler(const ServerConfig& cfg) : RequestHandlerBase(cfg) {}
    ~ErrorHandler() {};

    Response handle_request(const Request& r)
    {
        const vector<char>& body = get_resource(r);
        return Response(status, responseHeaders, body);
    };

private:
    const vector<char> get_resource(const Request& r)
    {
        return make_error_body(r.get_status());
    };
};

#endif // ERROR_HANDLER_HPP
