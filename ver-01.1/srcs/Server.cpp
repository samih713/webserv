#include "Server.hpp"
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __DEBUG
#include <iostream>
#endif // __DEBUG

Server &Server::getInstance()
{
    // avoid creating another one
    static Server instance;
    return instance;
}

// default constructor
Server::Server()
    : listener(
          socket(AF_INET, SOCK_STREAM, IPPROTO_TCP | SOCK_NONBLOCK)) // start the listener
    , nConn(0)
{
    // On success, a file descriptor for the new socket is returned.  On error, -1 is
    // returned, and errno is set
    if (listener == -1)
    {
        std::stringstream errorMessage;
        errorMessage << SOCKET_ERR << ": " << strerror(errno);
        throw std::runtime_error(errorMessage.str());
    }
#ifdef __DEBUG // if needed
    else
        std::cout << "Listener was created successfully fd[" << listener << "]\n";
#endif // __DEBUG
}

Server::~Server()
{
    close(listener);
}

Server::Server(const __attribute__((unused)) Server &other)
{}

Server &Server::operator=(const __attribute__((unused)) Server &other)
{
    return *this;
}

// member functions
void Server::start()
{
    while (true)
    {
        // int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
        // struct timeval *timeout);
        //
        // void FD_CLR(int fd, fd_set *set);
        // int  FD_ISSET(int fd, fd_set *set);
        // void FD_SET(int fd, fd_set *set);
        // void FD_ZERO(fd_set * set);

        // !! work on select
        // if (select(nConn, ))
        std::cout << "server is live boy!" << std::endl;
    }
}
