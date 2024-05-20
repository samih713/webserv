#include "Request.hpp"
#include "debug.hpp"

Request::Request() : status(OK) {}

Request::~Request() {}

Request::Request(const Request& other)
    : status(other.status), header(other.header), message(other.message.str()),
      body(other.body)
{}


const string& Request::get_method() const
{
    return header.method;
}

const HeaderMap& Request::get_headers() const
{
    return header.fields;
}

const string& Request::get_body() const
{
    return body;
}

const string& Request::get_resource() const
{
    return header.resource;
}

STATUS_CODE Request::get_status() const
{
    return status;
}

void Request::set_status(STATUS_CODE s)
{
    status = s;
}

ostream& operator<<(ostream& os, const Request& r)
{
    os << "Method: " << r.header.method << std::endl;
    os << "Request-Target: " << r.header.resource << std::endl;
    os << "HTTP-Version: " << r.header.version << std::endl;
    os << "************ fields *************\n";
    for (HeaderMap::const_iterator it = r.header.fields.begin();
         it != r.header.fields.end(); ++it)
        os << "[" << it->first << "]" << ": " << it->second << std::endl;
    os << "************ body *************\n";
    os << r.body << endl;
    return os;
}

// deleted copy assigment
void Request::operator=(const Request&)
{
    DEBUGASSERT("call to deleted 'Request' copy assignment");
}
