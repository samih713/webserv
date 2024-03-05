#include "Server.hpp"
#include "IRequestHandler.hpp"
#include "Request.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Returns the singleton instance of the Server class, creating it if necessary.
 *
 * @param listenerPort The port number to listen on
 * @param backlog The maximum length of the queue of pending connections
 * @return A reference to the singleton instance of the Server class
 */
Server &Server::getInstance(int listenerPort, int backlog)
{
    static Server instance(listenerPort, backlog);
    return instance;
}

/**
 * Constructs a new Server object with the specified listener port and backlog.
 *
 * @param listenerPort The port number to listen on
 * @param backlog The maximum length of the queue of pending connections
 */
Server::Server(fd listenerPort, int backlog)
    : listener_fd(listener.get_fd())
    , listener_port(listenerPort)
{
    listener.set_port(listener_port);
    listener.bind();
    listener.listen(backlog);
}

/**
 * Destroys the server closing its listener socket
 */
Server::~Server()
{
    ::close(listener.socket_descriptor);
}

/**
 * Handles a connection on the given socket.
 *
 * This function reads data from the socket, creates an HTTP request object,
 * determines the appropriate request handler based on the request method,
 * and generates an HTTP response. The response is then written back to the socket.
 *
 * @param recv_socket The file descriptor of the socket to handle the connection on.
 * @throws std::runtime_error if an error occurs while receiving data from the socket.
 */
static void handle_connection(fd recv_socket)
{
    char buffer[BUFFER_SIZE];
    int  bytesReceived = recv(recv_socket, &buffer[0], BUFFER_SIZE, 0);

    if (bytesReceived == -1)
        throw std::runtime_error(strerror(errno));

    string message(&buffer[0], &buffer[0] + bytesReceived);
    try
    {
        Request request(message);
        IRequestHandler *handler =
            RequestHandlerFactory::MakeRequestHandler(request.get_method());
        Response response = handler->handle_request(request);
        response.write_response(recv_socket);
        delete handler;
    }
    catch (std::runtime_error &e)
    {
        cerr << e.what();
    }
}

/**
 * This function uses the select system call to monitor multiple file descriptors for read
 * readiness.
 *
 * @throws std::runtime_error if an error occurs during the a system call
 */
void Server::select_strat()
{
    fd_set current_sockets;
    fd_set ready_to_read;

    FD_ZERO(&current_sockets);
    // adds the listener to the set
    FD_SET(listener_fd, &current_sockets);
    while (true) // main server loop
    {
        DEBUG_MSG(wait_message, R);
        // creates a secondary set (select is destructive)
        ready_to_read = current_sockets;
        // check all sockets in the set (only checking for ready to read)
        if (select(FD_SETSIZE, &ready_to_read, NULL, NULL, NULL) < 0 && (errno = 2))
            throw std::runtime_error(strerror(errno));
        // looping upto FD_SETSIZE(system default/needs to change)
        for (fd recv_socket = 0; recv_socket < FD_SETSIZE; recv_socket++)
        {
            if (FD_ISSET(recv_socket, &ready_to_read))
            {
                if (recv_socket == listener_fd)
                {
                    fd client_socket = listener.accept();
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

/**
 * Starts the server using the specified polling strategy.
 *
 * @param strategy The polling strategy to use (SELECT, KQUEUE, POLL, EPOLL)
 * @throws InvalidStrategyException if an invalid polling strategy is provided
 */
void Server::start(enum polling_strat strategy)
{
    switch (strategy)
    {
        case SELECT: select_strat(); break;
        // case KQUEUE: kqueue_strat(); break;
        // case POLL: poll_strat(); break;
        // case EPOLL: epoll_strat(); break;
        default: throw std::runtime_error("Invalid strategy"); break;
    }
}
