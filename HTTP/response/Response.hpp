#include "Message.hpp"
#include "Request.hpp"
#include "webserv.hpp"
#include <sstream>
#include <string>

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

namespace webserv
{
namespace http
{

class Response
{
    public:
        Response(STATUS_CODE status, vsp headers, string body);
        ~Response();
        Response(const Response &other);
        const string generate_response(const Request &req);

    private:
        STATUS_CODE status;
        vsp         headers;
        string      body;
        // deleted copy assignment
        void operator=(const Response &);
};

};     // http
};     // webserv
#endif // RESPONSE_HPP
