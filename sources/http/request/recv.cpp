#include "Request.hpp"
#include "Server.hpp"


// TODO use expectedBodySize in recv instead of during parsing
/**
 * @brief receives data from the specified socket and processes it. It reads the
 * data into a buffer, checks for any errors, and updates the message accordingly. It also
 * checks if the header is ready to be processed.
 *
 * @param socket The file descriptor of the socket to receive data from
 * @return The connection status after processing the received data (CLOSE_CONNECTION or
 * KEEP_ALIVE)
 *
 * @throws std::runtime_error if an error occurs during the receive operation
 */
void Request::recv(fd socket)
{
    static char buffer[BUFFER_SIZE];
    int         bytesReceived = 0;

    std::memset(buffer, 0, BUFFER_SIZE);
    bytesReceived = ::recv(socket, &buffer[0], BUFFER_SIZE - 1, 0);

    if (bytesReceived == 0)
        THROW_EXCEPTION_WITH_INFO(R "no data recieved - closing connection" RE);

    if (bytesReceived == -1)
        THROW_EXCEPTION_WITH_INFO(R "Recieve failed:" + string(strerror(errno)) + RE);

    if (!message.str().empty()) // not the first recieve
        message.str().append(buffer, bytesReceived);
    else // first recieve
        message.str(string(buffer).substr(0, bytesReceived));

    // if header end is found, indicate header is ready to be parsed
    if (header(NOT_READY) && message.str().find(CRLF + CRLF))
        header.state = READY_TO_PARSE;
}
