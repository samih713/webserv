#include "Server.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

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

static void handle_connection(fd __attribute__((unused)) recv_socket)
{
    assert(false && "not implemented");
    // char buffer[HTTP::MSG_SIZE];

    // // 0 out the buffer
    // ::memset(&buffer, 0, HTTP::MSG_SIZE);

    // if ((recv(recv_socket, &buffer, HTTP::MSG_SIZE - 1, 0) == -1))
    //     throw std::runtime_error(strerror(errno));
    // HTTP::Context message(buffer);
    // std::cout << message.get_message() << std::endl;
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

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == _listenerFD)
                {
                    fd client_socket = _listener.accept();
                    FD_SET(client_socket, &current_sockets);
                }
                else
                {
                    DEBUG_MSG("reading from connection", M);
                    handle_connection(i);
                    FD_CLR(i, &current_sockets);
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
