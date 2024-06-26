#include "Response.hpp"
#include "Server.hpp"

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
Response::Response(STATUS_CODE status, const HeaderMap& headers, const vector<char>& body)
    : sent(false), status(status), headers(headers), body(body)
{
    LOG_DEBUG("Response: called for status: " + ws_itoa(status));
}

/**
 * @brief Destructor for Response class
 *
 * This destructor cleans up any resources used by the Response object.
 */
Response::~Response() {}

/**
 * @brief Copy constructor for Response class
 *
 * This constructor creates a new Response object by copying the contents of another
 * Response object.
 *
 * @param other The Response object to be copied
 */
Response::Response(const Response& other)
    : sent(other.sent), status(other.status), headers(other.headers), body(other.body)
{
    LOG_DEBUG("Response: Copy constructor called");
}

// TODO move to its own file
inline void Response::load_status_line(ostringstream& os) const
{
    DEBUGASSERT(status_codes_map.find(status) != status_codes_map.end());
    os << HTTP_VER << SP << status << SP << status_codes_map.find(status)->second << CRLF;
}

inline void Response::load_headers(ostringstream& os) const
{
    HeaderMap::const_iterator begin = headers.begin();
    HeaderMap::const_iterator end   = headers.end();
    for (HeaderMap::const_iterator it = begin; it != end; it++)
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
    int           result    = 0;

    load_status_line(os);
    load_headers(os);
    string headerString = os.str();
    // add the header
    vector<char> message(headerString.begin(), headerString.end());
    // add the body
    message.insert(message.end(), body.begin(), body.end());
    //! send all but not in a loop, so it doesn't block
    while (bytesSent < message.size()) {
        result = send(recv_socket, &message[bytesSent], message.size() - bytesSent, 0);
        if (result == -1)
            THROW_EXCEPTION_WITH_INFO(ERR_SEND_FAIL + strerror(errno));
        bytesSent += result;
    }
}

// deleted copy assignment
void Response::operator=(const Response&)
{
    DEBUGASSERT("call to deleted 'Response' copy assignment");
}
