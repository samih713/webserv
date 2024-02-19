#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Message.hpp"
#include "webserv.hpp"

namespace webserv
{
namespace http
{

// Request class to parse an incoming message
class Request
{
    public:
        Request(const string &rawRequest);
        ~Request();
        Request(const Request &other);
        METHOD          get_method() const;
        const string   &get_resource() const;
        const vps      &get_headers() const;
        friend ostream &operator<<(ostream &os, const Request &r);

    private:
        void         parse();
        const string rawRequest;
        // request-line   = method SP request-target SP HTTP-version
        METHOD method;
        string resource;
        string http_version;
        // field-line   = field-name ":" OWS field-value OWS
        vps header_fields;
        vps trailer_fields;
        // clang-format on
        std::string body;
        // deleted copy assigment
        void operator=(const Request &);
};


}; // namespace http
}; // namespace webserv

#endif // REQUEST_HPP
