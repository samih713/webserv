#include "Request.hpp"

// ! CGI
Request::Request()
    : cgiStatus(NOT_SET), cgiReadFd(NOT_SET), cgiClient(NOT_SET), cgiChild(9999999),
      status(OK)
{}

Request::~Request() {}

Request::Request(const Request& other)
    : cgiStatus(other.cgiStatus), cgiReadFd(other.cgiReadFd), cgiClient(other.cgiClient),
      cgiChild(other.cgiChild), status(other.status), header(other.header),
      message(other.message.str()), body(other.body)
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

const string& Request::get_query_string() const
{
    return header.queryString;
}

const string& Request::get_resource_path() const
{
    return resourcePath;
}

const string& Request::get_location_match() const
{
    return locationMatch;
}

void Request::set_status(STATUS_CODE s)
{
    status = s;
}

ostream& operator<<(ostream& os, const Request& r)
{
    os << "Method: " << r.header.method << endl;
    os << "Request-Target: " << r.header.resource << endl;
    os << "Request-Path: " << r.resourcePath << endl;
    if (!r.get_query_string().empty())
        os << "Query: " << r.get_query_string() << endl;
    os << "HTTP-Version: " << r.header.version << endl;
    os << "************ fields *************\n";
    for (HeaderMap::const_iterator it = r.header.fields.begin();
         it != r.header.fields.end(); ++it)
        os << "[" << it->first << "]" << ": " << it->second << endl;
    os << "************ body *************\n";
    os << r.body << endl;
    return os;
}

// deleted copy assigment
void Request::operator=(const Request&)
{
    DEBUGASSERT("call to deleted 'Request' copy assignment");
}
