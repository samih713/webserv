#include "Server.hpp"
#include "IRequestHandler.hpp"
#include "Request.hpp"
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using std::string;

Server &Server::getInstance(int listenerPort, int backlog)
{
    // avoid creating another one
    static Server instance(listenerPort, backlog);
    return instance;
}

// default constructor
Server::Server(fd listenerPort, int backlog)
    : _listenerFD(_listener.get_fd())
    , _listener_port(listenerPort)
{
    // setup the Server
    _listener.set_port(_listener_port);
    _listener.bind();
    _listener.listen(backlog);
}

Server::~Server()
{
    ::close(_listener.socket_descriptor);
}

static void handle_connection(fd recv_socket)
{
    char buffer[BUFFER_SIZE];
    int  bytesReceived = recv(recv_socket, &buffer[0], BUFFER_SIZE, 0);

    if (bytesReceived == -1)
        throw std::runtime_error(strerror(errno));

    string                 message(&buffer[0], &buffer[0] + bytesReceived);
    webserv::http::Request request(message);
    IRequestHandler       *handler =
        RequestHandlerFactory::MakeRequestHandler(request.get_method());
    Response response = handler->handle_request(request);

    response.write_response(recv_socket);

    delete handler;
}

#if 1
#undef FD_SETSIZE
#define FD_SETSIZE 24
#endif

void Server::__select_strat()
{
    fd_set current_sockets;
    fd_set ready_sockets;

    FD_ZERO(&current_sockets);

    FD_SET(_listenerFD, &current_sockets);
    while (true)
    {
        DEBUG_MSG(wait_message, R);

        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0 && (errno = 2))
            throw std::runtime_error(strerror(errno));

        for (fd recv_socket = 0; recv_socket < FD_SETSIZE; recv_socket++)
        {
            if (FD_ISSET(recv_socket, &ready_sockets))
            {
                if (recv_socket == _listenerFD)
                {
                    fd client_socket = _listener.accept();
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    DEBUG_MSG("reading from connection", M);
                    handle_connection(recv_socket);
                    FD_CLR(recv_socket, &current_sockets);
                }
            }
        }
    }
}

void Server::start(enum polling_strat strategy)
{
    switch (strategy)
    {
        case SELECT: __select_strat(); break;
        default: break;
    }
}
