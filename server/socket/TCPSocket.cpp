#include "TCPSocket.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

// hello world test
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

    DEBUG_MSG("TCPSocket created successfully", C);
}

/* [DESTRUCTOR] */

TCPSocket::~TCPSocket()
{
    DEBUG_MSG("TCPSocket closed!!", R);
}


