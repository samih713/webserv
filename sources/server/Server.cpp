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
#if defined(__MAC__)
#include <sys/event.h>
#endif
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
    : _listener(config.listenerPort, backLog, config.serverAddr), _config(config),
      _cachedPages(new CachedPages(config))
{
    DEBUG_MSG("Server was created successfully", B);
}

/* ------------------------------- DESTRUCTOR ------------------------------- */

/**
 * Destroys the server deleting cahcedPages
 */
Server::~Server()
{
    delete _cachedPages;
}

/* ---------------------------- HANDLE CONNECTION --------------------------- */

void Server::handle_connection(fd incoming, fd_set& activeSockets)
{
    try {
        ConnectionManager::check_connection(incoming);
        Request& r = ConnectionManager::add_connection(incoming, activeSockets);
        r.recv(incoming);
        if (!r.parse_request(_config))
            return;

        IRequestHandler* handler =
            RequestHandlerFactory::MakeRequestHandler(r.get_method());
        Response response = handler->handle_request(r, _cachedPages, _config);

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
    const fd listenerFd = _listener.get_fd();

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
                    incoming            = _listener.accept();
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

/* --------------------------------- KQUEUE --------------------------------- */

#if defined(__LINUX__)
void Server::kqueue_strat()
{
    THROW_EXCEPTION_WITH_INFO("Kqueue does not work on Linux");
}
#elif defined(__MAC__)
void Server::kqueue_strat()
{
    const fd              socketFD      = listener.get_fd();
    const struct timespec kqueueTimeOut = { .tv_sec = KQUEUEWAITTIME, .tv_nsec = 0 };

    struct kevent changeList;            // list of events to monitor
    struct kevent eventList[MAX_EVENTS]; // list of events that have occurred

    // creating kqueue
    const int kq = kqueue();
    if (kq == -1)
        THROW_EXCEPTION_WITH_INFO(strerror(errno));

    // adding socketFD to changeList to monitor for read events
    EV_SET(&changeList, socketFD, EVFILT_READ, EV_ADD, 0, 0, 0);

    // adding changeList containing only socketFD to kqueue
    if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
        close(kq);
        THROW_EXCEPTION_WITH_INFO(strerror(errno));
    }

    int numEvents = 0;
    while (true) // event loop
    {
        DEBUG_MSG(WAIT_MESSAGE, L);

        numEvents = kevent(kq, NULL, 0, eventList, MAX_EVENTS, &kqueueTimeOut);
        if (numEvents == -1) {
            close(kq);
            THROW_EXCEPTION_WITH_INFO(strerror(errno));
        }
        else if (numEvents == 0) // timeout
            continue;

        for (int i = 0; i < numEvents; i++) {
            if (eventList[i].ident == static_cast<uintptr_t>(socketFD)) {
                // add new connection to kqueue
                fd newConnection = _listener.accept();
                EV_SET(&changeList, newConnection, EVFILT_READ, EV_ADD, 0, 0, 0);
                if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
                    close(kq);
                    THROW_EXCEPTION_WITH_INFO(strerror(errno));
                }
            }
            else if (eventList[i].flags & EV_EOF) {
                // remove connection from kqueue
                EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
                if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
                    close(kq);
                    THROW_EXCEPTION_WITH_INFO(strerror(errno));
                }
                close(eventList[i].ident);
                // this should be done in handle_connection
            }
            else if (eventList[i].flags & EVFILT_READ) {
                DEBUG_MSG("reading from connection", M);
                //! this is not a good fix at all and is very hard-cody
                try {
                    Request req;
                    req.recv(eventList[i].ident);
                    if (!req.parse_request(config))
                        continue;

                    IRequestHandler* handler =
                        RequestHandlerFactory::MakeRequestHandler(req.get_method());
                    Response response =
                        handler->handle_request(req, _cachedPages, _config);
                    response.send_response(eventList[i].ident);
                    delete handler;
                } catch (std::ios_base::failure& f) {
                    DEBUG_MSG(ERR_PARSE, R);
                } catch (std::exception& e) {
                    EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE, 0, 0,
                        0);
                    if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
                        close(kq);
                        THROW_EXCEPTION_WITH_INFO(strerror(errno));
                    }
                    DEBUG_MSG(e.what(), R);
                }
            }
        }
    }

    close(kq);
}
#endif

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
        case KQUEUE: kqueue_strat(); break;
        // case POLL: poll_strat(); break;
        // case EPOLL: epoll_strat(); break;
        default: THROW_EXCEPTION_WITH_INFO("Invalid strategy"); break;
    }
}
