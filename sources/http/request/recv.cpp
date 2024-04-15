#include "../../server/Server.hpp"
#include "Request.hpp"

CONNECTION_STATUS Request::recv(fd socket)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int  bytesReceived;

    bytesReceived = ::recv(socket, &buffer[0], BUFFER_SIZE - 1, 0);

    if (bytesReceived == 0)
    {
        DEBUG_MSG("No data recieved: closing connection", R);
        return CLOSE_CONNECTION;
    }

    if (bytesReceived == -1)
    {
        DEBUG_MSG("Recieve failed:" + string(strerror(errno)), R);
        return CONNECTION_ERROR;
    }

    rawRequest.append(buffer, bytesReceived);

    if (rawRequest.find(CRLF + CRLF) != string::npos)
        headerReady = true;

    return KEEP_ALIVE;
}
