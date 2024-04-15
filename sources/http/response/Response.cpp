#include "Response.hpp"
#include "../../../includes/debug.hpp"
#include "../../../includes/enum_utils.hpp"
#include "../../server/Server.hpp"

/**
 * This constant string represents the HTTP version "HTTP/1.1" that is used in the
 * Response class. It is a static member of the Response class and cannot be modified.
 */
const string &Response::version = "HTTP/1.1";

/**
 * @brief Creates a map of status codes and their corresponding string representations.
 *
 * This function creates a map where the keys are status codes and the values are their
 * string representations. The status codes are defined in the STATUS_CODE_LIST macro.
 *
 * @return A StatusCodeMap object containing the status codes and their string
 * representations.
 */
static StatusCodeMap createStatusCodeMap()
{
    StatusCodeMap map;
#define X(a) map.insert(std::make_pair(a, #a));
    STATUS_CODE_LIST
#undef X
    return map;
}

const StatusCodeMap status_codes_map = createStatusCodeMap();


/**
 * @brief Constructor for Response class
 *
 * This constructor initializes a Response object with the provided status code, headers,
 * and body.
 *
 * @param status The status code of the response
 * @param headers The headers of the response
 * @param body The body of the response
 */
Response::Response(STATUS_CODE status, const vsp &headers, const vector<char> &body)
    : status(status)
    , headers(headers)
    , body(body)
{
    DEBUG_MSG("Response called\n", Y);
}

/**
 * @brief Destructor for Response class
 *
 * This destructor cleans up any resources used by the Response object.
 */
Response::~Response()
{}

/**
 * @brief Copy constructor for Response class
 *
 * This constructor creates a new Response object by copying the contents of another
 * Response object.
 *
 * @param other The Response object to be copied
 */
Response::Response(const Response &other)
    : status(other.status)
    , headers(other.headers)
    , body(other.body)
{
    DEBUG_MSG("Copy constructor called\n", Y);
}


// TODO move to its own file
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

/**
 * Sends the response to the client over the specified socket.
 *
 * This function constructs the response message by combining the status line, headers,
 * and body, and then sends the entire message over the provided socket.
 *
 * @param recv_socket The file descriptor of the socket to send the response over.
 * @throws std::runtime_error if an error occurs while sending the response.
 */
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
            throw std::runtime_error(ERR_SEND_FAIL + strerror(errno));
        bytesSent += result;
    }
}

// deleted copy assignment
void Response::operator=(const Response &)
{
    DEBUGASSERT("call to deleted 'Response' copy assignment");
}
