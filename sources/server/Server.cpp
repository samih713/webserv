#include "Server.hpp"

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
Server& Server::get_instance(ServerConfig& config, int backLog)
{
    static Server instance(config, backLog);
    string        host = inet_ntoa(*(struct in_addr*) &config.host);
    LOG_INFO("Server created successfully on port [" + host + ":" + ws_itoa(config.port) + "]");
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
Server::Server(ServerConfig& config, int backLog)
    : _listener(config.port, backLog, config.host), _config(config),
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

// TODO this function does not work with kqueue, only select
void Server::handle_connection(fd incoming, fd_set& activeSockets)
{
    try {
        ConnectionManager::check_connection(incoming);
        Request& r = ConnectionManager::add_connection(incoming, activeSockets);
        // int      id;

        r.recv(incoming);
        if (!r.process(_config))
            return;

        // if (r.get_resource().find("/cgi-bin") != string::npos) { //! cgi check
        //     DEBUG_MSG("Handling CGI", M);
        //     id = fork();
        //     if (id == -1)
        //         cerr << "Error forking process: " << strerror(errno) << endl; //!
        //     else if (id == 0) {
        //         // Child process
        //         IRequestHandler* handler = MakeRequestHandler(r.get_method());
        //         Response response = handler->handle_request(r, *_cachedPages, _config);
        //         response.send_response(incoming);
        //         ConnectionManager::remove_connection(incoming,
        //             activeSockets); // after completing remove
        //         delete handler;
        //         exit(0);
        //     }
        // }
        // else {
        IRequestHandler* handler  = MakeRequestHandler(r.get_method());
        Response         response = handler->handle_request(r, *_cachedPages, _config);
        response.send_response(incoming);
        delete handler;
    } catch (std::exception& error) {
		LOG_ERROR(error.what());
        DEBUG_MSG(__FILE__ << ":" << __LINE__ << ": " << R << "error: " << RE
                           << error.what(), R);
    }
    ConnectionManager::remove_connection(incoming, activeSockets);
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

        // TODO need to implement writeFds
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
    const fd              socketFD      = _listener.get_fd();
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
                    if (!req.process(_config))
                        continue;

                    IRequestHandler* handler = MakeRequestHandler(req.get_method());
                    Response         response =
                        handler->handle_request(req, *_cachedPages, _config);
                    response.send_response(eventList[i].ident);
                    delete handler;
                } catch (std::ios_base::failure& f) {
                    DEBUG_MSG(ERR_PARSE, R);
                } catch (std::exception& error) {
                    EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE, 0, 0,
                        0);
                    if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
                        close(kq);
                        THROW_EXCEPTION_WITH_INFO(strerror(errno));
                    }
                    DEBUG_MSG(error.what(), R);
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
