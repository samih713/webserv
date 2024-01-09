#include "Socket.hpp"
#include <cerrno>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <fcntl.h>

/*  [CONSTRUCTORS] */

Socket::Socket(int family, int type, int protocol, int flags)
    : socket_descriptor(invalid_file_descriptor)
    , is_bound(false)
    , is_listening(false)
{
#if defined(__LINUX__)
    socket_descriptor = socket(family, type | flags, protocol);
#elif defined(__MAC__)
    socket_descriptor = socket(family, type , protocol);
	fcntl(socket_descriptor, F_SETFL, flags);
#endif //

    // On success, a file descriptor for the new socket is returned.
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));

#ifdef __DEBUG__
    else
        std::cerr << "Socket was created successfully fd[" << socket_descriptor << "]\n";
#endif // __DEBUG__

    // zero out the sockaddr struct
    memset(&address, 0, sizeof(address));
}

/* Call protocol by name */
Socket::Socket(int family, int type, const char *protocol_name, int flags)
    : socket_descriptor(invalid_file_descriptor)
    , is_bound(false)
    , is_listening(false)
{
    if (!protocol_name)
        throw Socket::Exception(Exception::compose_msg(ERR_NULL));

    // identify the protocol name
    std::string proto_name(protocol_name);
    for (size_t i = 0; i < proto_name.length(); i++)
        proto_name[i] = std::tolower(proto_name[i]);

    // continue with the rest of the code
    struct protoent *protocol = getprotobyname(proto_name.c_str());
    // check getprotobyname return
    if (!protocol)
        throw Socket::Exception(Exception::compose_msg(ERR_NULL));

#ifdef __DEBUG__
    std::cerr << "protocol official name is [" << protocol->p_name << "]\n";
#endif // __DEBUG__

    socket_descriptor = socket(family, type | flags, protocol->p_proto);

    // On success, a file descriptor for the new socket is returned.
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));

#ifdef __DEBUG__
    else
        std::cerr << "Socket was created successfully fd[" << socket_descriptor << "]\n";
#endif // __DEBUG__

    // zero out the sockaddr struct
    memset(&address, 0, sizeof(address));
}

/* [DESTRUCTOR] */

Socket::~Socket()
{
#ifdef __DEBUG__
    std::cerr << "socket fd[" << socket_descriptor << "] closed!!\n";
#endif // __DEBUG__
    if (socket_descriptor != invalid_file_descriptor)
    {
        close(socket_descriptor);
        // invalidate the filedescriptor
        socket_descriptor = invalid_file_descriptor;
    }
}


/* [INTERFACE] */

file_descriptor Socket::get_fd() const
{
    return socket_descriptor;
}

void Socket::bind() const
{
    int status = ::bind(socket_descriptor, (struct sockaddr *)&address, sizeof(address));
    if (status == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_BIND));
    is_bound = true;
#ifdef __DEBUG__
    std::cerr << "Socket was bound successfully to port["
              << ntohs(((struct sockaddr_in *)&address)->sin_port) << "]\n";
#endif // __DEBUG__
}

void Socket::listen(int backlog) const
{
    if (!is_bound)
        throw Socket::Exception(Exception::compose_msg(ERR_NBIND));
    // global namespace to avoid conflict
    int status = ::listen(socket_descriptor, backlog);
    if (status == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_LIST));
    is_listening = true;

#ifdef __DEBUG__
    std::cerr << "Socket listen was successful, [" << socket_descriptor
              << "] is now ready to accept max backlog of [" << backlog
              << "] connections\n";
#endif // __DEBUG__
}


file_descriptor Socket::accept()
{
    // check if socket is listening
    if (!is_listening)
        throw Socket::Exception(Exception::compose_msg(ERR_NLIST));
    // This structure is filled in with the address of the peer socket,
    // as known to the communications layer.
    struct sockaddr peer_info; // does this need to be stored?
    socklen_t       peer_length = sizeof(peer_info);
    // The accept() system call is used with connection-based socket types (SOCK_STREAM,
    // SOCK_SEQPACKET).
    // it extracts the first pending connection request in the backlog que form listen.
    // creates a new connected socket, and returns a new file descriptor
    file_descriptor connected_socket;
    connected_socket = ::accept(socket_descriptor, &peer_info, &peer_length);
    if (connected_socket == invalid_file_descriptor)
    {
        // if set to non_blocking it returns EAGAIN or EWOULDBLOCK if no connection
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            throw Socket::Exception(Exception::compose_msg(ERR_ACCP));
#ifdef __DEBUG__
        else
            std::cerr << "Socket has no incoming connections \n";
#endif // __DEBUG__
    }
#ifdef __DEBUG__
    std::cerr << "Socket accepted a connection from \n";
#endif // __DEBUG__
    return connected_socket;
}
