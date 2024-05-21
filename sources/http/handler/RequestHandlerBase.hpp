#include "IRequestHandler.hpp"

#ifndef REQUESTHANDLERBASE_HPP
#define REQUESTHANDLERBASE_HPP

// Base class for request handlers
class RequestHandlerBase: public IRequestHandler {
public:
    RequestHandlerBase() {};
    ~RequestHandlerBase() {};
    void add_header(pair<string, string> header_field)
    {
        response_headers.push_back(header_field);
    }

protected:
    STATUS_CODE  status;
    vector<char> body;
    vsp          response_headers;
};

#endif // REQUESTHANDLERBASE_HPP
