#include "Logger.hpp"
#include "webserv.hpp"

#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

/**
 * Set the socket flag for non-blocking I/O operations
 * based on the operating system.
 */
#if defined(__LINUX__) // SOCK_FLAG
static const int SOCK_FLAG = SOCK_NONBLOCK;
#elif defined(__MAC__)
static const int SOCK_FLAG = O_NONBLOCK;
#else
static const int SOCK_FLAG = 0;
#endif

/**
 * @brief A non-blocking SOCK_STREAM, TCP(IP4) socket class
 *
 * @class TCPSocket
 * This class represents a TCP socket that uses the SOCK_STREAM protocol and operates on
 * IPv4 addresses.
 */
class TCPSocket {
public:
    TCPSocket(fd port, int backlog, in_addr_t serverIP);
    ~TCPSocket() throw();
    TCPSocket(const TCPSocket& other);
    TCPSocket& operator=(const TCPSocket&);

    void set_port(int port);
    fd   get_fd() const;
    void bind() const;
    void listen(int backlog) const;
    fd   accept() const;

    bool operator<(const TCPSocket& other) const
    {
        // LOG_INFO(ws_itoa(this));
        // LOG_INFO(ws_itoa(&other));
        return this < &other;
        // return this->get_fd() < other.get_fd();
    }

private:
    static const int family    = AF_INET;     // The address family for the socket (IPv4)
    static const int type      = SOCK_STREAM; // The socket type (stream-oriented)
    static const fd  invalidFD = -1;

    struct sockaddr address;
    fd              socketFD;
    mutable bool    isBound;
    mutable bool    isListening;
};

#endif // TCPSOCKET_HPP
