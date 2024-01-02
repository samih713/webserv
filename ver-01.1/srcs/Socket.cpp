#include "Socket.hpp"
#include <cerrno>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

Socket::Socket(int domain, int type, int protocol)
    : socket_descriptor(invalid_file_descriptor)
{
    socket_descriptor = socket(domain, type, protocol);
    // On success, a file descriptor for the new socket is returned.  On error, -1 is
    // returned, and errno is set
    if (socket_descriptor == invalid_file_descriptor)
    {
        std::stringstream errorMessage;
        errorMessage << SOCKET_ERR << ": " << strerror(errno);
        throw std::runtime_error(errorMessage.str());
    }
#ifdef __DEBUG__ // if needed
    else
        std::cout << "Listener was created successfully fd[" << listener << "]\n";
#endif // __DEBUG__
}

Socket::~Socket()
{
    if (socket_descriptor != invalid_file_descriptor)
    {
        close(socket_descriptor);
        socket_descriptor = invalid_file_descriptor; // invalidate the filedescriptor
    }
}


// member functions

WS_CODE Socket::bind(int family, int port, int address)
{
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_family = family;
    addr.sin_port = port;
    if (::bind(socket_descriptor, (struct sockaddr *)&addr, sizeof(addr)))
        return WS_BIND_FAIL;
    return WS_OK;
}

/**
 * Listens for a connection on a socket and marks the socket referred to by
 * socket_descriptor as a passive socket.
 *
 * @param backlog The maximum length to which the queue of pending connections for sockfd
 * may grow.
 *
 * @return WS_CODE The status code indicating the result of the listen operation.
 *
 * @throws None
 *
 * @note The listen operation is typically used on server sockets after the socket has
 * been bound to a specific address and port using the bind function. Once the listen
 * operation is successful, the server socket can accept incoming connections using the
 * accept function.
 */
WS_CODE Socket::listen(int backlog)
{
    // global namespace to avoid conflict
    int status = ::listen(socket_descriptor, backlog);
    if (status == -1)
        return WS_LISTEN_FAIL;
    return WS_OK;
}

// deleted but can't cause is 98 maguy
Socket::Socket(const Socket &other)
    : socket_descriptor(0)
{
    (void)other;
}

Socket &Socket::operator=(const Socket &other)
{

    (void)other;
    return *this;
}
