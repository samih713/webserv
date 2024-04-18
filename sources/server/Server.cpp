/* -------------------------------- INCLUDES -------------------------------- */
#include "Server.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "Request.hpp"
#include "RequestHandlerFactory.hpp"
#include "debug.hpp"
#include <algorithm>
#include <sys/select.h>
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
Server& Server::get_instance(const ServerConfig& config, int backLog)
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
Server::Server(const ServerConfig& config, int backLog)
    : listener(config.listenerPort, backLog), config(config),
      cachedPages(new CachedPages(config))
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

void Server::handle_connection(fd incoming, fd_set& activeSockets)
{
    try {
        ConnectionManager::check_connection(incoming);
        Request& r = ConnectionManager::add_connection(incoming, activeSockets);
        r.recv(incoming);
        if (!r.parse_request(config))
            return;

        IRequestHandler* handler =
            RequestHandlerFactory::MakeRequestHandler(r.get_method());
        Response response = handler->handle_request(r, cachedPages, config);

        response.send_response(incoming);
        r.setCompleted();
        delete handler;
    } catch (std::ios_base::failure& f) {
        DEBUG_MSG(ERR_PARSE, R);
    } catch (std::exception& e) {
        ConnectionManager::remove_connection(incoming, activeSockets);
        DEBUG_MSG(e.what(), R);
    }
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

    fd       incoming;
    const fd listenerFd = listener.get_fd();

    fd_set activeSockets;
    fd_set readytoRead;

    FD_ZERO(&activeSockets);
    FD_SET(listenerFd, &activeSockets);

    // Initialize with listenerFd
    fd maxSocketDescriptor = listenerFd;

    while (true) {
        DEBUG_MSG(WAIT_MESSAGE, L);

        ConnectionManager::remove_expired(activeSockets);
        readytoRead = activeSockets;

        if (select(maxSocketDescriptor + 1, &readytoRead, NULL, NULL, &selectTimeOut) < 0)
            THROW_EXCEPTION_WITH_INFO(strerror(errno));
        selectTimeOut.tv_sec = SELECTWAITTIME;

        for (fd currentSocket = 0; currentSocket <= maxSocketDescriptor; currentSocket++)
        {
            if (FD_ISSET(currentSocket, &readytoRead)) {
                if (currentSocket == listenerFd) {
                    incoming            = listener.accept();
                    maxSocketDescriptor = std::max(maxSocketDescriptor, incoming);
                }
                else
                    incoming = currentSocket;
                DEBUG_MSG("reading from connection", M);
                handle_connection(incoming, activeSockets);
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
    switch (strategy) {
        case SELECT: select_strat(); break;
        // case KQUEUE: kqueue_strat(); break;
        // case POLL: poll_strat(); break;
        // case EPOLL: epoll_strat(); break;
        default: THROW_EXCEPTION_WITH_INFO("Invalid strategy"); break;
    }
}
