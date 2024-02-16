#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Message.hpp"
#include <map>
#include <ostream>
#include <string>
#include <vector>

using std::istream;
using std::map;
using std::ostream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

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
        void            parse();
        friend ostream &operator<<(ostream &os, const Request &r);
        METHOD          get_method() const;

    private:
        const string rawRequest;
        // request-line   = method SP request-target SP HTTP-version
        METHOD method;
        string resource;
        string http_version;
        // clang-format off
        // field-line   = field-name ":" OWS field-value OWS
        vector<pair<string, string> > header_fields;
        vector<pair<string, string> > trailer_fields;
        // clang-format on
        std::string body;
        // deleted copy assigment
        void operator=(const Request &);
};


}; // namespace http
}; // namespace webserv

#endif // REQUEST_HPP
