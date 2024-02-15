#include "Request.hpp"
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
    , request_target(other.request_target)
    , http_version(other.http_version)
    , fields(other.fields)
{}

namespace webserv
{
namespace http
{
std::ostream &operator<<(ostream &os, const Request &r)
{
    os << "Method: " << webserv::enumToString(r.method) << std::endl;
    os << "Request-Target: " << r.request_target << std::endl;
    os << "HTTP-Version: " << r.http_version << std::endl;
    os << "************ fields *************\n";
    for (size_t i = 0; i < r.fields.size(); i++)
    {
        os << "[" << r.fields[i].first << "]"
           << ": " << r.fields[i].second << std::endl;
    }
    return os;
}
}; // namespace http
}; // namespace webserv
