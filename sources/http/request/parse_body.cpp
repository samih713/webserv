#include "Message.hpp"
#include "Request.hpp"
#include "debug.hpp"
#include "webserv.hpp"

#define IS_SET(x) ((x) != NOT_SPECIFIED)

// TODO handle chunked encoding
static bool parse_chunked_body()
{
    DEBUGASSERT(false && "not yet implemented");
    return false;
    // when done set parsed to true
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
