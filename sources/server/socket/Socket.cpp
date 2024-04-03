#include "Socket.hpp"

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
    : socketFD(invalidFD)
    , isBound(false)
    , isListening(false)
{

#if defined(__LINUX__)
    socketFD = socket(family, type | flags, protocol);
#elif defined(__MAC__)
    socketFD = socket(family, type, protocol);
    flags |= fcntl(socketFD, F_GETFL, 0);
    fcntl(socketFD, F_SETFL, flags);
#endif // conditional

    if (socketFD == invalidFD)
        throw Socket::Exception(ERR_CREAT);

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
Socket::~Socket() throw()
{

    if (socketFD != invalidFD)
    {
        DEBUG_MSG("socket fd[" << socketFD << "] closed!!", R);
        close(socketFD);
        socketFD = invalidFD;
    }
}

/* -------------------------------- Interface ------------------------------- */

#define MAX_PORT 65535

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
    if (port < 0 || port > MAX_PORT)
        throw Socket::Exception("Invalid Socket descriptor\n");

    // cast address to socketaddr_in and set sin_port
    ((struct sockaddr_in *)(&address))->sin_port = htons(port);
}


fd Socket::get_fd() const throw()
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
    if (::bind(socketFD, (struct sockaddr *)&address, sizeof(address)) == -1)
        throw Socket::Exception(ERR_BIND);
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
        throw Socket::Exception(ERR_NBIND);

    if (::listen(socketFD, backlog) == -1)
        throw Socket::Exception(ERR_LIST);
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
        throw Socket::Exception(ERR_NLIST);

    // This structure is filled in with the address of the peer socket,
    struct sockaddr peerInfo; // does this need to be stored?
    socklen_t       peerLength = sizeof(peerInfo);

    // The accept() system call is used with connection-based socket types (SOCK_STREAM,
    // SOCK_SEQPACKET).
    // it extracts the first pending connection request in the backlog que form listen.
    fd newSocket;
    newSocket = ::accept(socketFD, &peerInfo, &peerLength);
    if (newSocket == invalidFD)
    {
        // if set to non_blocking it returns EAGAIN or EWOULDBLOCK if no connection
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            throw Socket::Exception(ERR_ACCP);
    }
    return newSocket;
}

/* ---------------------------- Socket Exception ---------------------------- */

/**
 * @brief Constructor for the Socket Exception class
 *
 * This constructor initializes a new instance of the Socket Exception class with the
 * provided error message.
 *
 * @param error_message The error message associated with the exception
 * @return A new instance of the Socket Exception class
 *
 * @throws None
 */
Socket::Exception::Exception(const std::string &error_message)
{
    this->error_message = compose_msg(error_message);
};

/**
 * @brief Returns a C-style string describing the error message associated with this
 * exception.
 *
 * @return const char* a C-style string containing the error message.
 */
const char *Socket::Exception::what() const throw()
{
    return error_message.c_str();
}

/**
 * Composes an error message by combining the provided message with the system error
 * message corresponding to the current value of errno.
 *
 * @param message The message to be included in the error message.
 * @return A string containing the composed error message.
 */
std::string Socket::Exception::compose_msg(const std::string &message)
{
    std::stringstream _msg;
    _msg << message << (errno ? ": " + std::string(std::strerror(errno)) : "");
    return _msg.str();
}
