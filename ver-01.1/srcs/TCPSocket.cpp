#include "TCPSocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

TCPSocket::TCPSocket()
    : Socket(family, type, 0, SOCK_NONBLOCK)
{
    // zero out the sockaddr struct before copying
    std::memset(&address, 0, sizeof(address));
    struct sockaddr_in addr;

    // prep up the struct
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = family;
    std::memcpy(&address, &addr, sizeof(addr));
}

void TCPSocket::set_port(int port) throw()
{
    std::memset(&address, 0, sizeof(address));
    struct sockaddr_in addr;
    addr.sin_port = htons(port);
    std::memcpy(&address, &addr, sizeof(addr));
}

TCPSocket::~TCPSocket()
{
#ifdef __DEBUG__
    std::cerr << "TCPSocket closed!!\n";
#endif // __DEBUG__
}
