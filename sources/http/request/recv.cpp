#include "../../server/Server.hpp"
#include "Request.hpp"

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
    int  bytesReceived = 0;

    std::memset(buffer, 0, BUFFER_SIZE);
    bytesReceived = ::recv(socket, &buffer[0], BUFFER_SIZE - 1, 0);

    if (bytesReceived == 0)
        THROW_EXCEPTION_WITH_INFO(R "no data recieved - closing connection" RE);

    if (bytesReceived == -1)
        THROW_EXCEPTION_WITH_INFO(R "Recieve failed:" + string(strerror(errno)) + RE);

    if (!message.str().empty())
        message.str().append(buffer, bytesReceived);
    else
        message.str(string(buffer).substr(0, bytesReceived));

    if (message.str().find(CRLF + CRLF) != string::npos)
        headerState = READY_TO_PARSE;
}
