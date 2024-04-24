#include "Message.hpp"
#include "Request.hpp"
#include "debug.hpp"
#include "webserv.hpp"

#define IS_SET(x) ((x) != NOT_SET)

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
    int available(0);
    int toAppendSize(0);

    // error checking
    DEBUGASSERT(header.bodySize >= recievedBodyLength &&
                "negative remaining size check logic:" __FILE__);

    int remaining(header.bodySize - recievedBodyLength);
    // chunked encoding
    if (header.chunked) {
        return (parse_chunked_body());
    }
    // content-length parsing
    else if (IS_SET(header.bodySize)) {
        available    = message.str().length() - message.tellg();
        toAppendSize = std::min(available, remaining);
        body.append(message.str().substr(message.tellg()), toAppendSize);
    }
    else
        return false;
    return true;
}
