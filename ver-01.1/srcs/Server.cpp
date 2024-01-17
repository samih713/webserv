#include "../includes/Server.hpp"
#include <chrono>
#include <cstddef>
#include <cstring>
#include <future>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

// TODO // [P]artially implemented, needs [I]mprovement, [X] done
//
// [ ] Handle Socket exceptions here?
// [ ] Implement select

Server &Server::getInstance(int listenerPort, int backlog)
{
    // avoid creating another one
    static Server instance(listenerPort, backlog);
    return instance;
}

// default constructor
Server::Server(int listenerPort, int backlog)
    : listenerPort(listenerPort)
    , status(0)
{
    // setup the Server
    listener.set_port(listenerPort);
    listener.bind();
    listener.listen(backlog);
}

Server::~Server()
{
    ::close(listener.socket_descriptor);
}


void Server::start()
{

    // wait duration before displaying a wait message, needs lpthread and 11
    // only for testing
    auto _duration(std::chrono::seconds(5));

    // making 2 fd_sets because select is destructive?
    fd_set current_sockets;
    fd_set ready_sockets;

    // zero out the current_sockets
    FD_ZERO(&current_sockets);

    // adds the listener socket to the set of current sockets
    FD_SET(listener.get_fd(), &current_sockets);

    while (true)
    {
        DEBUG_MSG(wait_message, R);

        ready_sockets = current_sockets;

        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
            throw std::runtime_error(strerror(errno));

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                // if its our listener, then we got a new connection
                if (i == listener.get_fd())
                {
                    // we accept the new connection
                    file_descriptor client_socket = listener.accept();
                    // add the new socket to the watched list of currenct sockets
                    FD_SET(client_socket, &current_sockets);
                }
                // its an exisiting connections that is ready for reading
                else
                {
                  // handle the connection
                  DEBUG_MSG( "reading from connection", M);
                  // remove it from there once its done
                  FD_CLR(i, &current_sockets); 
                }
            }
        }
    
        // forced sleep
        std::this_thread::sleep_for(_duration);
    }
}
