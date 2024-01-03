#include "Socket.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */


/*  [CONSTRUCTORS] */

Socket::Socket(int family, int type, int protocol, int flags)
    : socket_descriptor(invalid_file_descriptor)
    , family(family)
    , type(type)
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

void Socket::bind(int port)
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
        throw Socket::Exception(Exception::compose_msg(ERR_BIND));
}

void Socket::listen(int backlog)
{
    // global namespace to avoid conflict
    int status = ::listen(socket_descriptor, backlog);
    if (status == -1)
        throw Socket::Exception(Exception::compose_msg(ERR_LIST));
}
