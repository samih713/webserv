#include "../includes/TCPSocket.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifdef __DEBUG__
#include <iostream>
#endif // __DEBUG__

TCPSocket::TCPSocket()
#if defined(__LINUX__)
    : Socket(family, type, 0, SOCK_NONBLOCK)
#elif defined(__MAC__)
    : Socket(family, type, 0, O_NONBLOCK)
#endif
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

void TCPSocket::set_port(uint port) throw()
{
	((struct sockaddr_in *)(&address))->sin_port = htons(port);
}

TCPSocket::~TCPSocket()
{
#ifdef __DEBUG__
    std::cerr << "TCPSocket closed!!\n";
#endif // __DEBUG__
}
