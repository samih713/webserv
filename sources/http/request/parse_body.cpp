#include "Logger.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include "request_utils.hpp"
#include "webserv.hpp"

#define IS_SET(x) ((x) != NOT_SPECIFIED)

#define NOT_FOUND -1

// TODO handle chunked encoding
static int get_chunk_size(stringstream& message)
{
    int chunkLength(-1); // not found yet
    message >> std::hex >> chunkLength;
    validate_terminator(message, CRLF); // consume end of line
    return chunkLength;
}

bool Request::parse_chunked_body()
{
    static int chunkLength(-1);
    size_t     chunkEnd;

    while (!message.eof()) {
        // attempt to get the chunk size
        if (chunkLength == NOT_FOUND && find_terminator(message, CRLF))
            chunkLength = get_chunk_size(message);

        // is there a complete chunk?
        chunkEnd = message.str().find(CRLF, message.tellg());
        if (chunkEnd == string::npos)
            return false;

        // get the chunk
        string chunk;
        chunk = message.str().substr(message.tellg(), chunkEnd - message.tellg());

        // check if the chunk is the right size
        if (static_cast<int>(chunk.length()) != chunkLength) {
            LOG_ERROR("invalid chunk size");
            message.setstate(std::ios::failbit);
        }

        // 0 chunk means end of body
        if (chunk.empty())
            return true;

        // append the chunk to the body
        body += chunk;
        message.ignore(chunkLength + 2); // ignore the chunk and CRLF
        chunkLength = -1;                // reset chunk size
    }
    return false;
}

// https://datatracker.ietf.org/doc/html/rfc7230#section-3.3.3
// TODO handle transfer size
bool Request::parse_body()
{
    DEBUGASSERT(!message.eof() && "available is negative");

    // chunked encoding
    if (header.chunked) {
        return (parse_chunked_body());
    }
    else {
        // content-length parsing
        size_t available(message.str().length() - message.tellg());
        if (available < header.bodySize)
            return false;
        else if (IS_SET(header.bodySize))
            body = message.str().substr(message.tellg(), header.bodySize);
    }
    return true;
}
