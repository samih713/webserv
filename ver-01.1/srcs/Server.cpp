#include "../includes/Server.hpp"
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __DEBUG
#include <iostream>
#endif // __DEBUG

Server &Server::getInstance(file_descriptor listener_port, int backlog)
{
    // avoid creating another one
    static Server instance(listener_port, backlog);
    return instance;
}

// default constructor
Server::Server(int _listener_port, int backlog)
    : listener_port(_listener_port)
    , status(0)
{
    // setup the Server
    listener.set_port(listener_port);
    listener.bind();
    listener.listen(backlog);
}

Server::~Server()
{
    ::close(listener.socket_descriptor);
}


// void Server::start()
// {
//     int backlog = 0;
//     int status = 0;

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

//         // accept a connection on a socket
//         struct sockaddr_storage clientAddr;
//         socklen_t               clientAddrSize = sizeof(clientAddr);
//         int                     clientSocket =
//             accept(listener, (struct sockaddr *)&clientAddr,
//             &clientAddrSize);
//         if (clientSocket == -1)
//         {
//             std::stringstream errorMessage;
//             errorMessage << SOCKET_ERR << ": " << strerror(errno);
//             throw std::runtime_error(errorMessage.str());
//         }

//         // handle the connection
//         handleConnection(clientSocket);
//     }
// }
