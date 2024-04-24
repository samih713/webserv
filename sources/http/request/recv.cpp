#include "Request.hpp"
#include "Server.hpp"


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

    if (bytesReceived == -1)
        THROW_EXCEPTION_WITH_INFO(R "Recieve failed:" + string(strerror(errno)) + RE);

    message << string(buffer, bytesReceived);

    // if header end is found, indicate header is ready to be parsed
    if (header(NOT_READY) && message.str().find(CRLF + CRLF))
        header.state = READY_TO_PARSE;
}
