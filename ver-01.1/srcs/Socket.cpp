#include "../includes/Socket.hpp"
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */

/*  [CONSTRUCTORS] */

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
#endif //

    // On success, a file descriptor for the new socket is returned.
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));

    DEBUG_MSG("Socket was created successfully fd[" << socket_descriptor << "]", Y);

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

    DEBUG_MSG("protocol official name is [" << protocol->p_name << "]", Y);

    socket_descriptor = socket(family, type | flags, protocol->p_proto);

    // On success, a file descriptor for the new socket is returned.
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));

    DEBUG_MSG("Socket was created successfully fd[" << socket_descriptor << "]", Y);

    // zero out the sockaddr struct
    memset(&address, 0, sizeof(address));
}

/* [DESTRUCTOR] */

Socket::~Socket()
{
    DEBUG_MSG("socket fd[" << socket_descriptor << "] closed!!", R);

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

    DEBUG_MSG("Socket was bound successfully to port["
                  << ntohs(((struct sockaddr_in *)&address)->sin_port) << "]\n",
              B);
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

    DEBUG_MSG("Socket listen was successful, ["
                  << socket_descriptor << "] is now ready to accept max backlog of ["
                  << backlog << "] connections\n",
              M);
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

        DEBUG_MSG("Socket has no incoming connections", D);
    }

    DEBUG_MSG("Socket accepted a connection from", W);

    return connected_socket;
}
