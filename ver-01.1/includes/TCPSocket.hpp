#include <Socket.hpp>
#include <sys/socket.h>

#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

class TCPSocket : public Socket
{

    public:
        TCPSocket();
        ~TCPSocket();

    private:
        TCPSocket(const TCPSocket &);
        TCPSocket &operator=(const TCPSocket &);
};

#endif // TCPSOCKET_HPP
