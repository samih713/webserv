#include "Message.hpp"
#include "Request.hpp"
#include "debug.hpp"
#include "request_utils.hpp"
#include "webserv.hpp"
#include <string>

#define IS_SET(x) ((x) != NOT_SPECIFIED)

// TODO handle chunked encoding
bool Request::parse_chunked_body()
{
    static size_t chunked_body_size(0);
    string        line;

    // parse chunk size
    // if line is not ending in CRLF fail
    if (!peek_line_terminator(message, CRLF))
        message.setstate(std::ios::failbit);
    message >> std::hex >> chunked_body_size; // take in size
    message.ignore(2);                        // ignore CRLF

    // parse chunk
    // while (!peek_line_terminator(stringstream & message, const string& check))

        DEBUGASSERT(false && "not yet implemented");
    return false;
}


// https://datatracker.ietf.org/doc/html/rfc7230#section-3.3.3
// TODO handle transfer size
bool Request::parse_body()
{
    size_t available(message.str().length() - message.tellg());
    DEBUGASSERT(!message.eof() && "available is negative");
    // ! what happens if it never finished? it closes automatically?
    if (available < header.bodySize)
        return false;

    // chunked encoding
    if (header.chunked)
        return (parse_chunked_body());
    // content-length parsing
    else if (IS_SET(header.bodySize))
        body = message.str().substr(message.tellg(), header.bodySize);
    return true;
}
