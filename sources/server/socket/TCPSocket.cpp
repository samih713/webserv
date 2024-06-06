#include "TCPSocket.hpp"

/* ------------------------------- Constructor ------------------------------ */

TCPSocket::TCPSocket(fd port, int backLog, in_addr_t serverIP)
    // : Socket(family, type, 0, SOCK_FLAG)
    : socketFD(invalidFD), isBound(false), isListening(false)
{
    int flag;
#if defined(__LINUX__)
    flag     = SOCK_FLAG;
    socketFD = socket(family, type | SOCK_FLAG, IPPROTO_TCP);
#elif defined(__MAC__)
    flag     = SOCK_FLAG;
    socketFD = socket(family, type, IPPROTO_TCP);
    flag |= fcntl(socketFD, F_GETFL, 0);
    fcntl(socketFD, F_SETFL, flag);
#endif // conditional

    std::memset(&address, 0, sizeof(address));

    int option = 1;
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if (socketFD == invalidFD)
        THROW_EXCEPTION_WITH_INFO(ERR_CREAT);

    struct sockaddr_in addr;

    // prep up the struct
    addr.sin_addr.s_addr = serverIP;
    addr.sin_family      = family;
    std::memcpy(&address, &addr, sizeof(addr));

    // set-up port
    set_port(port);
    bind();
    listen(backLog);

    LOG_DEBUG("TCPSocket: TCP socket " + ws_itoa(socketFD) + " created on port [" +
              ws_itoa(ntohs(((struct sockaddr_in*) &address)->sin_port)) + "]");
}

/* ------------------------------- Destructor ------------------------------- */

TCPSocket::~TCPSocket() throw()
{
    if (socketFD != invalidFD) {
        close(socketFD);
        LOG_DEBUG("TCPSocket: TCP socket " + ws_itoa(socketFD) + " closed");
        socketFD = invalidFD;
    }
}

/* ---------------------------- copy constructor --------------------------- */

TCPSocket::TCPSocket(const TCPSocket& other)
    : address(other.address), socketFD(dup(other.socketFD)), isBound(other.isBound),
      isListening(other.isListening)
{
    LOG_DEBUG("TCPSocket: TCP socket " + ws_itoa(socketFD) + " copied");
}

TCPSocket& TCPSocket::operator=(const TCPSocket& other) //?
{
    if (this != &other) {
        address     = other.address;
        socketFD    = dup(other.socketFD);
        isBound     = other.isBound;
        isListening = other.isListening;
    }
    return *this;
}

/* -------------------------------- Interface ------------------------------- */

void TCPSocket::set_port(int port)
{
    ((struct sockaddr_in*) (&address))->sin_port = htons(port);
}

fd TCPSocket::get_fd() const
{
    return socketFD;
}

void TCPSocket::bind() const
{
    if (::bind(socketFD, (struct sockaddr*) &address, sizeof(address)) == -1)
        THROW_EXCEPTION_WITH_INFO(ERR_BIND);
    isBound = true;
}

void TCPSocket::listen(int backlog) const
{
    if (!isBound)
        THROW_EXCEPTION_WITH_INFO(ERR_NBIND);

    if (::listen(socketFD, backlog) == -1)
        THROW_EXCEPTION_WITH_INFO(ERR_LIST);
    isListening = true;
}

fd TCPSocket::accept() const
{
    if (!isListening)
        THROW_EXCEPTION_WITH_INFO(ERR_NLIST);

    struct sockaddr peerInfo; // does this need to be stored?
    socklen_t       peerLength = sizeof(peerInfo);

    fd newSocket = ::accept(socketFD, &peerInfo, &peerLength);
    if (newSocket == invalidFD) {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
            THROW_EXCEPTION_WITH_INFO(ERR_ACCP);
    }
    return newSocket;
}
