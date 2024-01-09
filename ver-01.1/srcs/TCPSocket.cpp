#include "TCPSocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>

TCPSocket::TCPSocket()
#if defined(__LINUX__)
    : Socket(family, type, 0, SOCK_NONBLOCK)
#elif defined(__MAC__)
    : Socket(family, type, 0, O_NONBLOCK)
#endif
{
    // zero out the sockaddr struct before copying
    std::memset(&address, 0, sizeof(address));
    struct sockaddr_in addr;

    // prep up the struct
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = family;
    std::memcpy(&address, &addr, sizeof(addr));
#ifdef __DEBUG__
    std::cerr << "TCPSocket created successfully\n";
#endif // __DEBUG__
}

void TCPSocket::set_port(int port) throw()
{
    struct sockaddr_in addr;

	int port_offset = offsetof(struct sockaddr_in, sin_port);
    addr.sin_port = htons(port);
    std::memcpy(&address + port_offset, &addr, sizeof(addr.sin_port));
}

TCPSocket::~TCPSocket()
{
#ifdef __DEBUG__
    std::cerr << "TCPSocket closed!!\n";
#endif // __DEBUG__
}
