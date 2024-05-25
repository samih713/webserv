#include "Request.hpp"
#include "Server.hpp"

static bool find_header_end(const stringstream& message);

// TODO use expectedBodySize in recv instead of during parsing
/**
 * @brief Receives data from a socket.
 *
 * This function is responsible for receiving data from a socket and storing it in the
 * Request object.
 *
 * @param socket The file descriptor of the socket to receive data from.
 *
 * @throws std::runtime_error if an error occurs during the receive operation.
 */
void Request::recv(fd socket)
{
    static char buffer[BUFFER_SIZE];
    int         bytesReceived = 0;

    std::memset(buffer, 0, BUFFER_SIZE);
    bytesReceived = ::recv(socket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived == 0)
        THROW_EXCEPTION_WITH_INFO(R "no data recieved - closing connection" RE);

    message << string(buffer, bytesReceived > 0 ? bytesReceived : 0);

    if (header(NOT_READY) && find_header_end(message))
        header.state = READY_TO_PARSE;
}

static bool find_header_end(const stringstream& message)
{
    return message.str().find(CRLF + CRLF) != string::npos;
}
