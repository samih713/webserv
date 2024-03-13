#include "Socket.hpp"
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

Socket::Socket(int family, int type, int protocol, int flags)
    : socket_descriptor(invalid_file_descriptor)
    , is_bound(false)
    , is_listening(false)
{
#if defined(__LINUX__)
    socket_descriptor = socket(family, type | flags, protocol);
#elif defined(__MAC__)
    socket_descriptor = socket(family, type, protocol);
    flags |= fcntl(socket_descriptor, F_GETFL, 0);
    fcntl(socket_descriptor, F_SETFL, flags);
#endif // conditional
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));
    std::memset(&address, 0, sizeof(address));
    DEBUG_MSG("Socket was created successfully fd[" << socket_descriptor << "]", Y);
}

Socket::~Socket()
{

    if (socket_descriptor != invalid_file_descriptor)
    {
        DEBUG_MSG("socket fd[" << socket_descriptor << "] closed!!", R);
        close(socket_descriptor);
        socket_descriptor = invalid_file_descriptor;
    }
}


/* [INTERFACE] */
#define MAX_PORT 65535
// throws execpetions
void Socket::set_port(int port)
{
    if (port < 0 || port > MAX_PORT)
        throw Socket::Exception("Invalid Socket descriptor\n");
    ((struct sockaddr_in *)(&address))->sin_port = htons(port);
}

fd Socket::get_fd() const throw()
{
    return socket_descriptor;
}

void Socket::bind() const
{
    if (::bind(socket_descriptor, (struct sockaddr *)&address, sizeof(address)) == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_BIND));
    is_bound = true;
}

void Socket::listen(int backlog) const
{
    if (!is_bound)
        throw Socket::Exception(Exception::compose_msg(ERR_NBIND));

    if (::listen(socket_descriptor, backlog) == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_LIST));
    is_listening = true;
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
    if (!is_listening)
        throw Socket::Exception(Exception::compose_msg(ERR_NLIST));

    // This structure is filled in with the address of the peer socket,
    struct sockaddr peer_info; // does this need to be stored?
    socklen_t       peer_length = sizeof(peer_info);

    // The accept() system call is used with connection-based socket types (SOCK_STREAM,
    // SOCK_SEQPACKET).
    // it extracts the first pending connection request in the backlog que form listen.
    fd connected_socket;
    connected_socket = ::accept(socket_descriptor, &peer_info, &peer_length);
    if (connected_socket == invalid_file_descriptor)
    {
        // if set to non_blocking it returns EAGAIN or EWOULDBLOCK if no connection
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            throw Socket::Exception(Exception::compose_msg(ERR_ACCP));
    }
    return connected_socket;
}
