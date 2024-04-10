#include "Response.hpp"
#include "../../../includes/debug.hpp"
#include "../../../includes/enum_utils.hpp"
#include "../../server/Server.hpp"

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

inline void Response::load_status_line(ostringstream &os) const
{
    os << version << SP << status << SP << status_codes_map.find(status)->second << CRLF;
}

inline void Response::load_headers(ostringstream &os) const
{
    vsp::const_iterator begin = headers.begin();
    vsp::const_iterator end = headers.end();
    for (vsp::const_iterator it = begin; it != end; it++)
        os << it->first << ": " << it->second << CRLF;
    os << CRLF;
}

void Response::send_response(fd recv_socket) const
{
    ostringstream os;
    size_t        bytesSent = 0;
    int           result = 0;

    load_status_line(os);
    load_headers(os);
    string headerString = os.str();
    // add the header
    vector<char> message(headerString.begin(), headerString.end());
    // add the body
    message.insert(message.end(), body.begin(), body.end());
    // send all
    while (bytesSent < message.size())
    {
        result = send(recv_socket, &message[bytesSent], message.size() - bytesSent, 0);
        if (result == -1)
            throw std::runtime_error(strerror(errno));
        bytesSent += result;
    }
}

// deleted copy assignment
void Response::operator=(const Response &)
{
    DEBUGASSERT("call to deleted 'Response' copy assignment");
}
