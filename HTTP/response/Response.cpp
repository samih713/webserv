#include "Response.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include "ResponseBuilder.hpp"
#include "debug.hpp"
#include <sstream>

using std::ostringstream;
using namespace webserv::http;

Response::Response(STATUS_CODE status, vsp headers, string body)
    : status(status)
    , headers(headers)
    , body(body)
{
    DEBUG_MSG("Response called\n", Y);
}

Response::~Response()
{}

Response::Response(const Response &other)
    : status(other.status)
    , headers(other.headers)
    , body(other.body)
{}


const string Response::generate_response(const Request &req)
{
    (void)req;
    // status = ResponseBuilder::set_status_code();
    DEBUGASSERT("Not yet implemented");
    return string("");
}

// deleted copy assignment
void Response::operator=(const Response &)
{
    DEBUGASSERT("call to deleted 'Response' copy assignment");
}
