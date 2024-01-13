#include "../includes/TCPSocket.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifdef __DEBUG__
#include <iostream>
#endif // __DEBUG__

/*  [CONSTRUCTORS] */

TCPSocket::TCPSocket()
   : Socket(family, type, 0, SOCK_FLAG)
{
    struct sockaddr_in addr;

    // prep up the struct
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = family;
    std::memcpy(&address, &addr, sizeof(addr));

#ifdef __DEBUG__
    std::cerr << "TCPSocket created successfully\n";
#endif // __DEBUG__
}

/* [DESTRUCTOR] */

TCPSocket::~TCPSocket()
{
#ifdef __DEBUG__
    std::cerr << "TCPSocket closed!!\n";
#endif // __DEBUG__
}

void TCPSocket::set_port(int port) throw()
{
    DEBUGASSERT(port >= 0 && port <= 65535); // could be an exception
    ((struct sockaddr_in *)(&address))->sin_port = htons(port);
}
