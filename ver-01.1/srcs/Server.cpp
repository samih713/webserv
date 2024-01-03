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

// // default constructor
// Server::Server()
//     : listener{
//           Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, SOCK_NONBLOCK)) // start the
//           listener
//     , status(WS_OK)
// {}
//
// Server::~Server()
// {
//     close(listener);
// }
//
// Server::Server(const __attribute__((unused)) Server &other)
// {}
//
// Server &Server::operator=(const __attribute__((unused)) Server &other)
// {
//     return *this;
// }
//
//
// void Server::start()
// {
//     int backlog = 0;
//     int status = 0;
//
//     // bind the socket
//     while (true)
//     {
//         if (status) // an error has occured
//         {
//             // If a connection request arrives when the queue is full
//             std::stringstream errorMessage;
//             errorMessage << SOCKET_ERR << ": " << strerror(errno);
//             throw std::runtime_error(errorMessage.str());
//         }
//
//         // accept a connection on a socket
//         struct sockaddr_storage clientAddr;
//         socklen_t               clientAddrSize = sizeof(clientAddr);
//         int                     clientSocket =
//             accept(listener, (struct sockaddr *)&clientAddr, &clientAddrSize);
//         if (clientSocket == -1)
//         {
//             std::stringstream errorMessage;
//             errorMessage << SOCKET_ERR << ": " << strerror(errno);
//             throw std::runtime_error(errorMessage.str());
//         }
//
//         // handle the connection
//         handleConnection(clientSocket);
//     }
// }
