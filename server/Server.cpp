/* -------------------------------- INCLUDES -------------------------------- */
#include "Server.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "Request.hpp"
#include "RequestHandlerFactory.hpp"
#include "debug.hpp"
#include <algorithm>
#include <sys/select.h>
#include <utility>
/* -------------------------------- INCLUDES -------------------------------- */

/* ------------------------------- CONSTRUCTOR ------------------------------ */

/**
 * @brief Returns the singleton instance of the Server class,
 * creating it if necessary.
 *
 * @param config reference to config object, containing root,
 * default, error pages ... etc
 * @param backLog The maximum length of the queue of pending connections
 * @return A reference to the singleton instance of the Server class
 */
Server &Server::get_instance(const Config &config, int backLog)
{
    static Server instance(config, backLog);
    return instance;
}

/**
 * @brief Constructor for the Server class.
 *
 * This constructor initializes a Server object with the provided configuration and
 * backlog.
 *
 * @param config The configuration settings for the server.
 * @param backLog The maximum length of the queue of pending connections.
 *
 * @throws Socket::Exception if there is an issue with setting up the listener socket.
 */
Server::Server(const Config &config, int backLog)
    : listener(config.listenerPort, backLog)
    , config(config)
    , cachedPages(new CachedPages(config))
{
    DEBUG_MSG("Server was created successfully", B);
}

/* ------------------------------- DESTRUCTOR ------------------------------- */

/**
 * Destroys the server deleting cahcedPages
 */
Server::~Server()
{
    delete cachedPages;
}

/* ---------------------------- HANDLE CONNECTION --------------------------- */

// TODO [ ] hanlde partial sends and recieves

/**
 * Handles a connection on the given socket.
 *
 * This function reads data from the socket, creates an HTTP request object,
 * determines the appropriate request handler based on the request method,
 * and generates an HTTP response. The response is then written back to the socket.
 *
 * @param recvSocket The file descriptor of the socket to handle the connection on.
 *
 * @throws std::runtime_error if an error occurs while receiving data from the socket.
 */
bool Server::handle_connection(fd recvSocket)
{
    char buffer[BUFFER_SIZE] = { 0 };
    int  bytesReceived;
    try
    {
        bytesReceived = recv(recvSocket, &buffer[0], BUFFER_SIZE, 0);

        if (bytesReceived == -1)
            throw std::runtime_error(strerror(errno));

        if (bytesReceived == 0)
            return CLOSE_CONNECTION;

        string  message(&buffer[0], &buffer[0] + bytesReceived);
        Request request(message);
        IRequestHandler *handler =
            RequestHandlerFactory::MakeRequestHandler(request.get_method());
        Response response = handler->handle_request(request, cachedPages, config);
        response.write_response(recvSocket);
        delete handler;
    }
    catch (std::runtime_error &e)
    {
        DEBUG_MSG(e.what(), R);
    }
    return KEEP_ALIVE;
}

/* -------------------------------------------------------------------------- */
/*                                POLLING STRAT                               */
/* -------------------------------------------------------------------------- */

// define static member
ConnectionMap ConnectionManager::connectionMap;

/* --------------------------------- SELECT --------------------------------- */

void Server::select_strat()
{
    struct timeval selectTimeOut = { .tv_sec = SELECTWAITTIME, .tv_usec = 0 };

    fd       newConnection;
    const fd listenerFd = listener.get_fd();

    fd_set activeSockets;
    fd_set readytoRead;

    FD_ZERO(&activeSockets);
    FD_SET(listenerFd, &activeSockets);

    // Initialize with listenerFd
    fd maxSocketDescriptor = listenerFd;

    while (true)
    {
        DEBUG_MSG(WAIT_MESSAGE, L);

        ConnectionManager::remove_expired(activeSockets);
        readytoRead = activeSockets;

        if (select(maxSocketDescriptor + 1, &readytoRead, NULL, NULL, &selectTimeOut) < 0)
            throw std::runtime_error(strerror(errno));
        selectTimeOut.tv_sec = SELECTWAITTIME;

        for (fd currentSocket = 0; currentSocket <= maxSocketDescriptor; currentSocket++)
        {
            if (FD_ISSET(currentSocket, &readytoRead))
            {
                if (currentSocket == listenerFd)
                {
                    newConnection = listener.accept();
                    ConnectionManager::add_connection(newConnection, activeSockets);
                    maxSocketDescriptor = std::max(maxSocketDescriptor, newConnection);
                }
                else
                {
                    DEBUG_MSG("reading from connection", M);
                    if (handle_connection(currentSocket) != CLOSE_CONNECTION)
                        ConnectionManager::update_connection(currentSocket);
                    else
                        ConnectionManager::remove_connection(currentSocket,
                                                             activeSockets);
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
