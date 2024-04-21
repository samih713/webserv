#include "Request.hpp"
#include "debug.hpp"

Request::Request()
    : status(OK), message(""), headerState(NOT_READY), parsed(false), completed(false),
      expectedBodySize(NOT_SET), maxBodySize(DEFAULT_MAX_BODY_SIZE)
{
}

Request::~Request() {}

Request::Request(const Request& other)
    : status(other.status), message(other.message.str()), headerState(other.headerState),
      parsed(other.parsed), completed(other.completed),
      expectedBodySize(other.expectedBodySize), maxBodySize(other.maxBodySize),
      method(other.method), resource(other.resource), httpVersion(other.httpVersion),
      headerFields(other.headerFields), trailer_fields(other.trailer_fields)

{
}


const string& Request::get_method() const
{
    return method;
}

const HeaderMap& Request::get_headers() const
{
    return headerFields;
}

const string& Request::get_resource() const
{
    return resource;
}

STATUS_CODE Request::get_status() const
{
    return status;
}

void Request::set_status(STATUS_CODE s)
{
    status = s;
}

void Request::set_completed()
{
    completed = true;
}

bool Request::is_completed()
{
    return completed;
}

ostream& operator<<(ostream& os, const Request& r)
{
    os << "Method: " << r.method << std::endl;
    os << "Request-Target: " << r.resource << std::endl;
    os << "HTTP-Version: " << r.httpVersion << std::endl;
    os << "************ fields *************\n";
    for (HeaderMap::const_iterator it = r.headerFields.begin(); it != r.headerFields.end(); ++it) {
        os << "[" << it->first << "]" << ": " << it->second
           << std::endl;
    }
    os << "************ body *************\n";
    os << r.body << std::endl;
    return os;
}

// deleted copy assigment
void Request::operator=(const Request&)
{
    DEBUGASSERT("call to deleted 'Request' copy assignment");
}
