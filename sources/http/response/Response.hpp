#include "../Message.hpp"
#include "../request/Request.hpp"
#include "../../../includes/webserv.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

namespace webserv
{
namespace http
{

class Response
{
    public:
        Response(STATUS_CODE status, const vsp &headers, const vector<char> &body);
        ~Response();
        Response(const Response &other);
        void write_response(fd recv_socket) const;

    private:
        STATUS_CODE  status;
        vsp          headers;
        vector<char> body;
        // deleted copy assignment
        void operator=(const Response &);
        // response constants
        static const string &server_header_value;
        static const string &version;
};

};     // http
};     // webserv
#endif // RESPONSE_HPP
