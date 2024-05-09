#include "Request.hpp"
#include "debug.hpp"
#include "enum_utils.hpp"

Request::Request()
    : message(""), headerReady(false), parsed(false), completed(false),
      expectedBodySize(NOT_SET)
{}

Request::~Request() {}

Request::Request(const Request& other)
    : message(other.message.str()), headerReady(other.headerReady), parsed(other.parsed),
      completed(other.completed), expectedBodySize(other.expectedBodySize),
      method(other.method), resource(other.resource), http_version(other.http_version),
      header_fields(other.header_fields), trailer_fields(other.trailer_fields)

{}


METHOD Request::get_method() const
{
    return method;
}

const vsp& Request::get_headers() const
{
    return header_fields;
}

const string& Request::get_resource() const
{
    return resource;
}

const string& Request::get_body() const
{
    return body;
}

void Request::setCompleted()
{
    completed = true;
}

bool Request::isCompleted()
{
    return completed;
}

ostream& operator<<(ostream& os, const Request& r)
{
    os << "Method: " << enumToString(r.method) << endl;
    os << "Request-Target: " << r.resource << endl;
    os << "HTTP-Version: " << r.http_version << endl;
    os << "************ fields *************\n";
    for (size_t i = 0; i < r.header_fields.size(); i++) {
        os << "[" << r.header_fields[i].first << "]" << ": " << r.header_fields[i].second
           << endl;
    }
    os << "************ body *************\n";
    os << r.body << endl;
    return os;
}

// deleted copy assigment
void Request::operator=(const Request&)
{
    DEBUGASSERT("call to deleted 'Request' copy assignment");
}
