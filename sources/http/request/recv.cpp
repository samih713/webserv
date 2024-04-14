#include "../../server/Server.hpp"
#include "Request.hpp"

CONNECT_STATUS Request::recv(fd socket)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int  bytesReceived;

    bytesReceived = ::recv(socket, &buffer[0], BUFFER_SIZE - 1, 0);

    if (bytesReceived == 0)
        return CLOSE_CONNECTION;

    if (bytesReceived == -1)
    {
        DEBUG_MSG("recieve failed:" + string(strerror(errno)), R);
        return CLOSE_CONNECTION;
    }

    rawRequest.append(buffer, bytesReceived);
    if (rawRequest.find(CRLF + CRLF) != string::npos)
        headerReady = true;
    return KEEP_ALIVE;
}
