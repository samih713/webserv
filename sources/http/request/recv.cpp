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

    if (!message.str().empty())
        message.str().append(buffer, bytesReceived);
    else
        message.str((string(buffer).substr(0, bytesReceived)));
    // message.str().append(0); // this situation
    // message.str((string(buffer).substr(0, bytesReceived)));

    if (message.str().find(CRLF + CRLF) != string::npos)
        headerReady = true;

    return KEEP_ALIVE;
}
