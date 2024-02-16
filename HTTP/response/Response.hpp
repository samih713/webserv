#include "Request.hpp"
#include <string>

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

using std::string;

namespace webserv
{
namespace http
{

class Response
{
    public:
        Response(const Request &r);
        ~Response();
        Response(const Response &);
        STATUS_CODE generate_response();

    private:
        static const string &SP;
        const Request       &r;

        STATUS_CODE set_status_line();
        // deleted copy assignment
        void operator=(const Response &);
};

};     // http
};     // webserv
#endif // RESPONSE_HPP
