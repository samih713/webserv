#include "Message.hpp"
#include "Request.hpp"
#include "Response.hpp"

#ifndef RESPONSEBUILDER_HPP
#define RESPONSEBUILDER_HPP

using namespace webserv::http;

class ResponseBuilder
{
    public:
        ResponseBuilder(const Request &request);
        ~ResponseBuilder();
        const Response get_response();
        ResponseBuilder(const ResponseBuilder &other);

    private:
        void             set_status_code();
        STATUS_CODE      build_body() const;
        const Request   &request;
        ResponseBuilder &operator=(const ResponseBuilder &);
};

#endif // RESPONSEBUILDER_HPP
