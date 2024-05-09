#include "Socket.hpp"
#include "../../../includes/webserv.hpp"

/* ------------------------------- Constructor ------------------------------ */

/**
 * @brief Constructor for the Socket class
 *
 * This constructor initializes a Socket object with the specified parameters.
 *
 * @param family The address family (e.g. AF_INET for IPv4)
 * @param type The socket type (e.g. SOCK_STREAM for TCP)
 * @param protocol The protocol to be used (e.g. IPPROTO_TCP for TCP)
 * @param flags Additional flags to be used when creating the socket
 *
 * @throws Socket::Exception if the socket creation fails
 *
 * @return None
 */
Socket::Socket(int family, int type, int protocol, int flags)
    : socketFD(invalidFD), isBound(false), isListening(false)
{
#if defined(__LINUX__)
    socketFD = socket(family, type | flags, protocol);
#elif defined(__MAC__)
    socketFD = socket(family, type, protocol);
    flags |= fcntl(socketFD, F_GETFL, 0);
    fcntl(socketFD, F_SETFL, flags);
#endif // conditional
    // TODO - better option handling
    int option = 1;
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (socketFD == invalidFD)
        THROW_EXCEPTION_WITH_INFO(ERR_CREAT);

    std::memset(&address, 0, sizeof(address));

    DEBUG_MSG("Socket was created successfully fd[" << socketFD << "]", Y);
}


/**
 * @brief Destructor for the Socket class
 *
 * This function closes the socket file descriptor if it is not already closed.
 *
 * @return void
 */
Socket::~Socket()
{
    if (socketFD != invalidFD) {
        DEBUG_MSG("socket fd[" << socketFD << "] closed!!", R);
        close(socketFD);
        socketFD = invalidFD;
    }
}

/* -------------------------------- Interface ------------------------------- */

/**
 * Sets the port for the Socket object.
 *
 * This function sets the port for the Socket object to the specified port number.
 *
 * @param port An integer representing the port number to set for the Socket object.
 *
 * @throws Socket::Exception if the port number is not within the valid range (0 to
 * MAX_PORT).
 */
void Socket::set_port(int port)
{
    // cast address to socketaddr_in and set sin_port
    ((struct sockaddr_in*) (&address))->sin_port = htons(port);
}


fd Socket::get_fd() const
{
    return socketFD;
}

/**
 * Binds the socket to a specific address and port.
 *
 * This function binds the socket to the address and port specified in the 'address'
 * member variable.
 *
 * @throws Socket::Exception if the bind operation fails
 */
void Socket::bind() const
{
    if (::bind(socketFD, (struct sockaddr*) &address, sizeof(address)) == -1)
        THROW_EXCEPTION_WITH_INFO(ERR_BIND);
    isBound = true;
}

/**
 * Starts listening for incoming connections on the socket.
 *
 * @param backlog The maximum length of the queue of pending connections.
 * @throws Socket::Exception if the socket is not bound or if an error occurs while
 * listening.
 */
void Socket::listen(int backlog) const
{
    if (!isBound)
        THROW_EXCEPTION_WITH_INFO(ERR_NBIND);

    if (::listen(socketFD, backlog) == -1)
        THROW_EXCEPTION_WITH_INFO(ERR_LIST);
    isListening = true;
}


/**
 * Accepts a new incoming connection on the listening socket.
 *
 * This function checks if the socket is currently in a listening state. If not, it throws
 * a Socket::Exception with an error message.
 *
 * @return fd The file descriptor of the newly accepted connection.
 *
 * @throws Socket::Exception If the socket is not in a listening state or if an error
 * occurs during the accept() system call.
 */
fd Socket::accept()
{
    if (!isListening)
        THROW_EXCEPTION_WITH_INFO(ERR_NLIST);

    // This structure is filled in with the address of the peer socket,
    struct sockaddr peerInfo; // does this need to be stored?
    socklen_t       peerLength = sizeof(peerInfo);

    // The accept() system call is used with connection-based socket types (SOCK_STREAM,
    // SOCK_SEQPACKET).
    // it extracts the first pending connection request in the backlog que form listen.
    fd newSocket;
    newSocket = ::accept(socketFD, &peerInfo, &peerLength);
    if (newSocket == invalidFD) {
        // if set to non_blocking it returns EAGAIN or EWOULDBLOCK if no connection
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            THROW_EXCEPTION_WITH_INFO(ERR_ACCP);
    }
    return newSocket;
}
