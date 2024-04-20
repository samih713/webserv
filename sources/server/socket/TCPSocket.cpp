#include "TCPSocket.hpp"

#ifdef __DEBUG__
#include <iostream>
#endif // __DEBUG__

/**
 * @brief Constructor for creating a TCP socket with the specified port and backlog.
 *
 * This constructor initializes a TCPSocket object with the given port number and backlog
 * value. It sets up the socket address structure, binds the socket to the specified port,
 * and starts listening for incoming connections with the specified backlog.
 *
 * @param port The port number to bind the socket to.
 * @param backLog The maximum length of the queue of pending connections.
 *
 * @throws Socket::Exception if there is an error in creating or setting up the socket.
 */
TCPSocket::TCPSocket(int port, int backLog) : Socket(family, type, 0, SOCK_FLAG)
{
    struct sockaddr_in addr;
    // prep up the struct
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family      = family;
    std::memcpy(&address, &addr, sizeof(addr));

    // set-up port
    set_port(port);
    bind();
    listen(backLog);
    DEBUG_MSG("TCPSocket created successfully, listening on port [" + ws_itoa(port) + "]",
        L);
}


TCPSocket::~TCPSocket() throw()
{
    DEBUG_MSG("TCPSocket closed!!", R);
}
