#include "Response.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include "debug.hpp"
#include <sstream>

using std::ostringstream;
using namespace webserv::http;

const string &Response::SP = " ";

Response::Response(const Request &r)
    : r(r)
{}

Response::~Response()
{}

Response::Response(const Response &other)
    : r(other.r)
{
    (void)other;
}

// work on this
STATUS_CODE Response::set_status_line()
{
    ostringstream os;
    METHOD        request_method = r.get_method();
    os << "HTTP/1.1";
    switch (request_method)
    {
        case (GET): os << SP << OK << SP << "OK"; break;
        default: break;
    }
    return NOT_IMPLEMENTED;
}

STATUS_CODE Response::generate_response()
{
    DEBUGASSERT("Not yet implemented");
    ostringstream os;

    return NOT_IMPLEMENTED;
}
// deleted copy assignment
void Response::operator=(const Response &)
{
    DEBUGASSERT("call to deleted 'Response' copy assignment");
}
