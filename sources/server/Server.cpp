/* -------------------------------- INCLUDES -------------------------------- */
#include "./Server.hpp"
#include "../../includes/debug.hpp"
#include "../http/handler/GetRequestHandler.hpp"
#include "../http/handler/IRequestHandler.hpp"
#include "../http/handler/RequestHandlerFactory.hpp"
#include "../http/request/Request.hpp"
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
Server &Server::get_instance(const ServerConfig &config, int backLog)
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
Server::Server(const ServerConfig &config, int backLog)
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

// TODO [x] hanlde partial sends and recieves
//		- better naming for manager and set activeSockets in class

void Server::handle_connection(fd incoming, fd_set &activeSockets)
{
    // recieve the incoming connection
    Request *r;

    if (!ConnectionManager::check_connection(incoming))
    {
        ConnectionManager::remove_connection(incoming, activeSockets);
        return;
    }
    else
    {
        r = &(ConnectionManager::add_connection(incoming, activeSockets)).first->second;
        r->timer.update_time();
        if (r->recv(incoming) == CLOSE_CONNECTION)
        {
            ConnectionManager::remove_connection(incoming, activeSockets);
            return;
        }
    }

    // parse headers
    if (r->header_ready() && r->expected_body_size() == NOT_SET)
    {
        try
        {
            r->parse_header();
            if (r->expected_body_size() != NOT_SPECIFIED)
                r->parse_body();
        }
        catch (std::ios_base::failure &f)
        {
            DEBUG_MSG(f.what(), R);
            ConnectionManager::remove_connection(incoming, activeSockets);
            // TODO need to form proper error response in case of parsing failure
            return;
        }
    }

    if (!r->isParsed())
        return;
    try
    {
        IRequestHandler *handler =
            RequestHandlerFactory::MakeRequestHandler(r->get_method());
        Response response = handler->handle_request(*r, cachedPages, config);

        response.send_response(incoming);
        r->setCompleted();
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
                    incoming = listener.accept();
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
    switch (strategy)
    {
        case SELECT: select_strat(); break;
        // case KQUEUE: kqueue_strat(); break;
        // case POLL: poll_strat(); break;
        // case EPOLL: epoll_strat(); break;
        default: throw std::runtime_error("Invalid strategy"); break;
    }
}
