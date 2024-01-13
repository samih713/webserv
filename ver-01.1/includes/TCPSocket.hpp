#include "Socket.hpp"
#include <sys/socket.h>

// is-a inheritance relationship, every TCPSocket is a Socket
#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

// a non-blocking SOCK_STREAM, TCP(IP4) socket 
class TCPSocket: public Socket
{

    public:
        TCPSocket();
        void set_port(uint port) throw();
        ~TCPSocket();

    private:
        TCPSocket(const TCPSocket &);
        TCPSocket &operator=(const TCPSocket &);

        // TCPSocket parameters
        static const int family = AF_INET;
        static const int type = SOCK_STREAM;
};

#endif // TCPSOCKET_HPP
