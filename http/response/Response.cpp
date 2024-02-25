#include "Response.hpp"
#include "Message.hpp"
#include "debug.hpp"
#include "enum_utils.tpp"

using namespace webserv::http;

// response codes
typedef std::map<STATUS_CODE, std::string> StatusCodeMap;

StatusCodeMap createStatusCodeMap()
{
    StatusCodeMap map;
#define X(a) map.insert(std::make_pair(a, #a));
    STATUS_CODE_LIST
#undef X
    return map;
}

StatusCodeMap status_codes_map = createStatusCodeMap();

// default header the name of the server
const string &Response::server_header_value = "The Wired";

const string &Response::version = "HTTP/1.1";

Response::Response(STATUS_CODE status, const vsp &headers, const vector<char> &body)
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
{
    DEBUG_MSG("Copy constructor called\n", Y);
}


//! needs to be adjusted dont use write? for now only doing to test with cout
void Response::write_response(ostream &outFile) const
{
    ostringstream os;
    // status line
    os << version << SP << status << SP << status_codes_map.find(status)->second << CRLF;
    // headers
    vsp::const_iterator begin = headers.begin();
    vsp::const_iterator end = headers.end();
    for (vsp::const_iterator it = begin; it != end; it++)
        os << it->first << ": " << it->second << CRLF;
    os << CRLF;
    // output the header
    outFile << os.str();

    // output the body
    vector<char> v_body(body.begin(), body.end());
    outFile.write(body.data(), body.size());
}

// deleted copy assignment
void Response::operator=(const Response &)
{
    DEBUGASSERT("call to deleted 'Response' copy assignment");
}
