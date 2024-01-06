#include "Socket.hpp"
#include <cerrno>
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
    socket_descriptor = socket(family, type | flags, protocol);

    // On success, a file descriptor for the new socket is returned.
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));
#ifdef __DEBUG__
    else
        std::cout << "Socket was created successfully fd[" << socket_descriptor << "]\n";
#endif // __DEBUG__
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
    std::cout << "protocol official name is [" << protocol->p_name << "]\n";
#endif // __DEBUG__

    socket_descriptor = socket(family, type | flags, protocol->p_proto);

    // On success, a file descriptor for the new socket is returned.
    if (socket_descriptor == invalid_file_descriptor)
        throw Socket::Exception(Exception::compose_msg(ERR_CREAT));

#ifdef __DEBUG__
    else
        std::cout << "Socket was created successfully fd[" << socket_descriptor << "]\n";
#endif // __DEBUG__
}

/* [DESTRUCTOR] */

Socket::~Socket()
{
    if (socket_descriptor != invalid_file_descriptor)
    {
        close(socket_descriptor);
        // invalidate the filedescriptor
        socket_descriptor = invalid_file_descriptor;
    }
#ifdef __DEBUG__
    else
        std::cerr << "socket fd[" << socket_descriptor << "] closed!!\n";
#endif // __DEBUG__
}


/* [INTERFACE] */

file_descriptor Socket::get_fd() const
{
    return socket_descriptor;
}

    int status = ::bind(socket_descriptor, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_BIND));
    is_bound = true;
#ifdef __DEBUG__
    std::cerr << "Socket was bound successfully to port["
              << ntohs(((struct sockaddr_in *)&address)->sin_port) << "]\n";
#endif // __DEBUG__
}

void Socket::listen(int backlog)
{
    if (!is_bound)
        throw Socket::Exception(Exception::compose_msg(ERR_NBIND));
    // global namespace to avoid conflict
    int status = ::listen(socket_descriptor, backlog);
    if (status == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_LIST));
    is_listening = true;
}
