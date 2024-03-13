/* -------------------------------- INCLUDES -------------------------------- */
#include "Server.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "Request.hpp"
#include "RequestHandlerFactory.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
/* -------------------------------- INCLUDES -------------------------------- */

/* ------------------------------- CONSTRUCTOR ------------------------------ */

/**
 * Returns the singleton instance of the Server class, creating it if necessary.
 *
 * @param config reference to config object, containing root,
 * default, error pages ... etc
 * @param backlog The maximum length of the queue of pending connections
 * @return A reference to the singleton instance of the Server class
 */
Server &Server::get_instance(const Config &config, int backLog)
{
    static Server instance(config, backLog);
    return instance;
}

/**
 * Constructs a new Server object with the specified listener port and backlog.
 *
 * @param listenerPort The port number to listen on
 * @param backlog The maximum length of the queue of pending connections
 */
Server::Server(const Config &config, int backLog)
    : config(config)
    , listenerFd(listener.get_fd())
{
    listener.set_port(config.listenerPort);
    listener.bind();
    listener.listen(backLog);
    cachedPages = new CachedPages(config);
}

/* ------------------------------- DESTRUCTOR ------------------------------- */

/**
 * Destroys the server closing its listener socket
 */
Server::~Server()
{
    ::close(listener.socket_descriptor);
}

/* ---------------------------- HANDLE CONNECTION --------------------------- */

// buffer size
static const int BUFFER_SIZE = 4096;

/**
 * Handles a connection on the given socket.
 *
 * This function reads data from the socket, creates an HTTP request object,
 * determines the appropriate request handler based on the request method,
 * and generates an HTTP response. The response is then written back to the socket.
 *
 * @param recvSocket The file descriptor of the socket to handle the connection on.
 * @throws std::runtime_error if an error occurs while receiving data from the socket.
 */
void Server::handle_connection(fd recvSocket)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int  bytesReceived;
    try
    {
        bytesReceived = recv(recvSocket, &buffer[0], BUFFER_SIZE, 0);
        if (bytesReceived == -1)
            throw std::runtime_error(strerror(errno));
        string  message(&buffer[0], &buffer[0] + bytesReceived);
        Request request(message);
        // TODO [ ] compare bytesReceived with size from headers
        IRequestHandler *handler =
            RequestHandlerFactory::MakeRequestHandler(request.get_method());
        Response response = handler->handle_request(request, *cachedPages, config);
        response.write_response(recvSocket);
        delete handler;
    }
    catch (std::runtime_error &e)
    {
        DEBUG_MSG(e.what(), R);
    }
}

/* -------------------------------------------------------------------------- */
/*                                POLLING STRAT                               */
/* -------------------------------------------------------------------------- */

/* --------------------------------- SELECT --------------------------------- */

// set fd size only for testing
#undef FD_SETSIZE
#define FD_SETSIZE 24

// wait message
static const std::string wait_message("Server is now waiting for connections...\n");

/**
 * This function uses the select system call to monitor multiple file descriptors for read
 * readiness.
 *
 * @throws std::runtime_error if an error occurs during the select system call
 */
void Server::select_strat()
{
    fd_set currentSockets;
    fd_set readytoRead;

    FD_ZERO(&currentSockets);
    FD_SET(listenerFd, &currentSockets);
    while (true)
    {
        DEBUG_MSG(wait_message, R);
        readytoRead = currentSockets;
        if (select(FD_SETSIZE, &readytoRead, NULL, NULL, NULL) < 0 && (errno = 2))
            throw std::runtime_error(strerror(errno));
        for (fd recvSocket = 0; recvSocket < FD_SETSIZE; recvSocket++)
        {
            if (FD_ISSET(recvSocket, &readytoRead))
            {
                if (recvSocket == listenerFd)
                {
                    fd clientSocket = listener.accept();
                    FD_SET(clientSocket, &currentSockets);
                }
                else
                {
                    DEBUG_MSG("reading from connection", M);
                    handle_connection(recvSocket);
                    FD_CLR(recvSocket, &currentSockets);
                }
            }
        }
    }
}

/* ---------------------------------- START --------------------------------- */

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
