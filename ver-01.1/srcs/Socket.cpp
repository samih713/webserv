#include "Socket.hpp"
#include <cerrno>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>

#ifdef __DEBUG__
#include <cassert>
#include <iostream>
#define DEBUGASSERT(x) assert(x)
#else
#define DEBUGASSERT(x)                                                                   \
    do                                                                                   \
    {                                                                                    \
    } while (0)
#endif // __DEBUG__

/**
 * Creates a new socket with the specified domain, type, protocol, and flags.
 *
 * @param domain The communication domain for the socket. This selects the protocol
 * family.
 * @param type The type of socket to be created.
 * @param protocol The protocol to be used. By default, the protocol is set to 0.
 * @param flags The flags to be set for the socket. By default, the flags are set to
 * SOCK_NONBLOCK.
 *
 * @throws std::runtime_error if the socket creation fails.
 *
 * @note Normally, only one protocol exists to support a specific socket type within a
 * particular protocol family.
 */
Socket::Socket(int domain, int type, int protocol, int flags)
    : socket_descriptor(invalid_file_descriptor)
    , family(domain)
    , type(type)
{
    DEBUGASSERT(domain > 0);
    DEBUGASSERT(protocol >= 0);

    socket_descriptor = socket(domain, type | flags, protocol);

    // On success, a file descriptor for the new socket is returned.
    // On error, -1 is returned, and errno is set
    if (socket_descriptor == invalid_file_descriptor)
    {
        std::stringstream errorMessage;
        errorMessage << ERR_STRT_MSG << ": " << strerror(errno);
        throw std::runtime_error(errorMessage.str());
    }
#ifdef __DEBUG__
    else
        std::cout << "Socket was created successfully fd[" << socket_descriptor << "]\n";
#endif // __DEBUG__
}

// can call protocol by name
Socket::Socket(int domain, int type, const char *protocol_name, int flags)
    : socket_descriptor(invalid_file_descriptor)
{
    if (!protocol_name)
        throw std::runtime_error(ERR_NULL_MSG);

    DEBUGASSERT(domain > 0);

    // identify the protocol name
    std::string proto_name(protocol_name);
    for (size_t i = 0; i < proto_name.length(); i++)
        proto_name[i] = std::tolower(proto_name[i]);

    // continue with the rest of the code
    struct protoent *protocol = getprotobyname(proto_name.c_str());
    // check getprotobyname return
    if (!protocol)
        throw std::runtime_error(ERR_NULL_MSG);

#ifdef __DEBUG__
    std::cout << "protocol official name is [" << protocol->p_name << "]\n";
#endif // __DEBUG__


    socket_descriptor = socket(domain, type | flags, protocol->p_proto);

    // On success, a file descriptor for the new socket is returned.  On error, -1 is
    // returned, and errno is set
    if (socket_descriptor == invalid_file_descriptor)
    {
        std::stringstream errorMessage;
        errorMessage << ERR_STRT_MSG << ": " << strerror(errno);
        throw std::runtime_error(errorMessage.str());
    }
#ifdef __DEBUG__ // if needed
    else
        std::cout << "Socket was created successfully fd[" << socket_descriptor << "]\n";
#endif // __DEBUG__
}

// destructor
Socket::~Socket()
{
    if (socket_descriptor != invalid_file_descriptor)
    {
        close(socket_descriptor);
        socket_descriptor = invalid_file_descriptor; // invalidate the filedescriptor
    }
#ifdef __DEBUG__
    else
        std::cerr << "socket fd[" << socket_descriptor << "] closed!!\n";
#endif // __DEBUG__
}


// member functions
WS_CODE Socket::bind(int port)
{
    // this has to be created based on the type of socket
    struct sockaddr_in addr;

    // zero out the sockaddr struct
    memset(&addr, 0, sizeof(addr));
    // same for these filled they are determined based on the family type
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = family;
    addr.sin_port = htons(port);

    int status = ::bind(socket_descriptor, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1)
        return WS_BIND_FAIL;

    return WS_OK;
}

/**
 * Listens for a connection on a socket and marks the socket referred to by
 * socket_descriptor as a passive socket.
 *
 * @param backlog The maximum length to which the queue of pending connections for
 * sockfd may grow.
 *
 * @return WS_CODE The status code indicating the result of the listen operation.
 *
 * @throws None
 *
 * @note The listen operation is typically used on server sockets after the socket has
 * been bound to a specific address and port using the bind function. Once the listen
 * operation is successful, the server socket can accept incoming connections using
 * the accept function.
 */
WS_CODE Socket::listen(int backlog)
{
    // global namespace to avoid conflict
    int status = ::listen(socket_descriptor, backlog);
    if (status == -1)
        return WS_LISTEN_FAIL;

    return WS_OK;
}
