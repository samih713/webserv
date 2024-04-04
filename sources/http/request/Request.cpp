#include "./Request.hpp"
#include "../../../includes/debug.hpp"
#include "../../../includes/enum_utils.hpp"

using namespace webserv::http;

Request::Request(const string &rawRequest)
    : rawRequest(rawRequest)
{
    try
    {
        parse();
    }
    catch (std::ios_base::failure &f)
    {
        throw std::runtime_error("Invalid request\n");
    }
}

Request::~Request()
{}

Request::Request(const Request &other)
    : rawRequest(other.rawRequest)
    , method(other.method)
    , resource(other.resource)
    , http_version(other.http_version)
    , header_fields(other.header_fields)
{}


METHOD Request::get_method() const
{
    return method;
}

const vsp &Request::get_headers() const
{
    return header_fields;
}

const string &Request::get_resource() const
{
    return resource;
}

namespace webserv
{
namespace http
{
std::ostream &operator<<(ostream &os, const Request &r)
{
    os << "Method: " << enumToString(r.method) << std::endl;
    os << "Request-Target: " << r.resource << std::endl;
    os << "HTTP-Version: " << r.http_version << std::endl;
    os << "************ fields *************\n";
    for (size_t i = 0; i < r.header_fields.size(); i++)
    {
        os << "[" << r.header_fields[i].first << "]" << ": " << r.header_fields[i].second
           << std::endl;
    }
    os << "************ body *************\n";
    os << r.body << std::endl;
    return os;
}
}; // namespace http
}; // namespace webserv

// deleted copy assigment
void Request::operator=(const Request &)
{
    DEBUGASSERT("call to deleted 'Request' copy assignment");
}
