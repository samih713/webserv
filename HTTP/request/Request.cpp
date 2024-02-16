#include "Request.hpp"
#include "debug.hpp"
#include "enum_utils.tpp"

using namespace webserv::http;

Request::Request(const string &rawRequest)
    : rawRequest(rawRequest)
{}

Request::~Request()
{}

Request::Request(const Request &other)
    : rawRequest(other.rawRequest)
    , method(other.method)
    , resource(other.resource)
    , http_version(other.http_version)
    , header_fields(other.header_fields)
{}

namespace webserv
{
namespace http
{
std::ostream &operator<<(ostream &os, const Request &r)
{
    os << "Method: " << webserv::enumToString(r.method) << std::endl;
    os << "Request-Target: " << r.resource << std::endl;
    os << "HTTP-Version: " << r.http_version << std::endl;
    os << "************ fields *************\n";
    for (size_t i = 0; i < r.header_fields.size(); i++)
    {
        os << "[" << r.header_fields[i].first << "]"
           << ": " << r.header_fields[i].second << std::endl;
    }
    os << "************ body *************\n";
    os << r.body << std::endl;
    return os;
}

METHOD Request::get_method() const
{
    return method;
}

// deleted copy assigment
void Request::operator=(const Request &)
{
    DEBUGASSERT("call to deleted 'Request' copy assignment");
}
}; // namespace http
}; // namespace webserv
