#include "Socket.hpp"

#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

/**
 * @brief A non-blocking SOCK_STREAM, TCP(IP4) socket class
 *
 * This class represents a TCP socket that uses the SOCK_STREAM protocol and operates on
 * IPv4 addresses.
 *
 * @class TCPSocket
 */
class TCPSocket : public Socket
{

    public:
        TCPSocket();
        ~TCPSocket() throw();

    private:
        static const int family = AF_INET;   ///< The address family for the socket (IPv4)
        static const int type = SOCK_STREAM; ///< The socket type (stream-oriented)

        /* @brief Copy constructor for TCPSocket (private and not implemented) */
        TCPSocket(const TCPSocket &);
        /* @brief Assignment operator for TCPSocket (private and not implemented) */
        TCPSocket &operator=(const TCPSocket &);
};

#endif // TCPSOCKET_HPP
