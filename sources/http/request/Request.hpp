#include "../Message.hpp"
#include "../../../includes/webserv.hpp"

#ifndef REQUEST_HPP
#define REQUEST_HPP

// Request class to parse an incoming message
class Request
{
    public:
        Request(const string &rawRequest);
        ~Request();
        Request(const Request &other);
        METHOD          get_method() const;
        const string   &get_resource() const;
        const vsp      &get_headers() const;
        friend ostream &operator<<(ostream &os, const Request &r);

    private:
        void          parse_request();
        const string &rawRequest;
        // request-line   = method SP request-target SP HTTP-version
        METHOD method;
        string resource;
        string http_version;
        // field-line   = field-name ":" OWS field-value OWS
        vsp header_fields;
        vsp trailer_fields;
        // clang-format on
        std::string body;
        // deleted copy assigment
        void operator=(const Request &);
};

#endif // REQUEST_HPP
