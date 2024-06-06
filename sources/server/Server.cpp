#include "Server.hpp"
#include "ConnectionManager.hpp"

/* ------------------------------- CONSTRUCTOR ------------------------------ */

/**
 * @brief Constructor for the Server class.
 *
 * This constructor initializes a Server object with the provided configuration and
 * backlog.
 *
 * @param cfgs The configuration settings for the server. //todo rewrite
 * @param backLog The maximum length of the queue of pending connections.
 *
 * @throws Socket::Exception if there is an issue with setting up the listener socket.
 */
Server::Server(vector<ServerConfig>& cfgs, int backLog) : cp(new CachedPages(cfgs[0])) //!
{
    for (vector<ServerConfig>::iterator sc = cfgs.begin(); sc != cfgs.end(); sc++) {
        servers[TCPSocket(sc->port, backLog, sc->host)] = *sc;

        string s_host = inet_ntoa(*(struct in_addr*) &sc->host);
        LOG_INFO("Server created on port [" + s_host + ":" + ws_itoa(sc->port) + "]");
    }
    LOG_DEBUG("Server was created successfully");
}

/* ---------------------------- HANDLE CONNECTION --------------------------- */

// TODO this function does not work with kqueue, only select
void Server::handle_connection(fd incoming, ServerConfig& cfg)
{
    LOG_INFO("Server: handling connection for [" + ws_itoa(incoming) + "]");
	if (incoming == -1)
		return ;
	// DEBUGASSERT(incoming != -1);

    try {
        ConnectionManager::check_connection(incoming);

        Request& r = ConnectionManager::add_connection(incoming);
        r.recv(incoming); //! check if it loops in here again
        if (!r.process(cfg)) {
            // ConnectionManager::remove_connection(incoming);
            return;
        }

        IRequestHandler* handler = make_request_handler(r.get_method(), cfg);

        // ! need to check if request is allowed for specific resource
        Response response = handler->handle_request(r);

        if (r.cgiStatus == NOT_SET) {
            response.send_response(incoming);
            ConnectionManager::remove_connection(incoming);
        }
        else if (r.cgiStatus == IN_PROCESS) {
            r.cgiStatus = TEMP; // set to temp to avoid double processing
            r.cgiClient = incoming;
            ConnectionManager::add_cgi_connection(r);
			LOG_INFO("client [[["+ ws_itoa(r.cgiClient) + "]]]");
			LOG_INFO("read [[["+ ws_itoa(r.cgiReadFd) + "]]]");
        }
        else if (r.cgiStatus == COMPLETED) {
            LOG_DEBUG("CGI is completed, sending response");
            response.send_response(r.cgiClient);
            ConnectionManager::remove_connection(r.cgiClient);
            ConnectionManager::remove_connection(r.cgiReadFd);
        }
        delete handler;
    } catch (TimeOut::Exception& to) {
        LOG_INFO("Server: connection timed out for [" + ws_itoa(incoming) + "]");
        ConnectionManager::remove_connection(incoming);
    } catch (std::exception& error) {
        LOG_ERROR("Server: " + string(error.what()));
        ConnectionManager::remove_connection(incoming);
    }
}

/* -------------------------------------------------------------------------- */
/*                                POLLING STRAT                               */
/* -------------------------------------------------------------------------- */

// define static member
ConnectionMap ConnectionManager::connectionMap;
TCPfdMap      ConnectionManager::TCPMap;
fd_set        ConnectionManager::activeSockets;
fd            ConnectionManager::maxSD(0);

/* --------------------------------- SELECT --------------------------------- */

void Server::select_strat()
{
    struct timeval selectTimeOut = { .tv_sec = SELECTWAITTIME, .tv_usec = 0 };

    fd& maxSD = ConnectionManager::get_max_sd();
    fd  incoming;

    fd_set listenerFDs = ConnectionManager::add_listeners(servers);
    fd_set readytoRead;

	const TCPSocket* tcp;
    while (true) {
        LOG_INFO("Server: Waiting for connections ...");

        ConnectionManager::remove_expired();
        readytoRead = ConnectionManager::get_active_sockets();

        if (select(maxSD + 1, &readytoRead, NULL, NULL, &selectTimeOut) < 0)
            THROW_EXCEPTION_WITH_INFO(strerror(errno));

        selectTimeOut.tv_sec = SELECTWAITTIME;

        for (fd currentSocket = 0; currentSocket <= maxSD; currentSocket++) {
            if (FD_ISSET(currentSocket, &readytoRead)) {
                if (FD_ISSET(currentSocket, &listenerFDs)) {
					tcp = ConnectionManager::get_tcp(currentSocket);
                    incoming = tcp->accept();
					// LOG_INFO("incoming: [" + ws_itoa(incoming) + "]");
					ConnectionManager::add_fd_pair(tcp, incoming);
				}
                else {
                    incoming = currentSocket;
					tcp = ConnectionManager::get_tcp(currentSocket);
				}
                handle_connection(incoming, servers.at(*tcp));
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
    //     const fd              listenerFD    = listener.get_fd();
    //     const struct timespec kqueueTimeOut = { .tv_sec = KQUEUEWAITTIME, .tv_nsec = 0
    //     };

    //     struct kevent changeList;            // list of events to monitor
    //     struct kevent eventList[MAX_EVENTS]; // list of events that have occurred

    //     // creating kqueue
    //     const int kq = kqueue();
    //     if (kq == -1)
    //         THROW_EXCEPTION_WITH_INFO(strerror(errno));

    //     // adding listenerFD to changeList to monitor for read events
    //     EV_SET(&changeList, listenerFD, EVFILT_READ, EV_ADD, 0, 0, 0);

    //     // adding changeList containing only listenerFD to kqueue
    //     if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //         close(kq);
    //         THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //     }

    //     int numEvents = 0;
    //     while (true) // event loop
    //     {
    //         DEBUG_MSG(WAIT_MESSAGE, L);

    //         numEvents = kevent(kq, NULL, 0, eventList, MAX_EVENTS, &kqueueTimeOut);
    //         if (numEvents == -1) {
    //             close(kq);
    //             THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //         }
    //         else if (numEvents == 0) // timeout
    //             continue;

    //         for (int i = 0; i < numEvents; i++) {
    //             if (eventList[i].ident == static_cast<uintptr_t>(listenerFD)) {
    //                 // add new connection to kqueue
    //                 fd newConnection = listener.accept();
    //                 EV_SET(&changeList, newConnection, EVFILT_READ, EV_ADD, 0, 0, 0);
    //                 if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //                     close(kq);
    //                     THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //                 }
    //             }
    //             else if (eventList[i].flags & EV_EOF) {
    //                 // remove connection from kqueue
    //                 EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE, 0,
    //                 0, 0); if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //                     close(kq);
    //                     THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //                 }
    //                 close(eventList[i].ident);
    //                 // this should be done in handle_connection
    //             }
    //             else if (eventList[i].flags & EVFILT_READ) {
    //                 DEBUG_MSG("reading from connection", M);
    //                 //! this is not a good fix at all and is very hard-cody
    //                 try {
    //                     Request req;
    //                     req.recv(eventList[i].ident);
    //                     if (!req.process(cfg))
    //                         continue;

    //                     IRequestHandler* handler  =
    //                     make_request_handler(req.get_method()); Response response =
    //                     handler->handle_request(req);
    //                     response.send_response(eventList[i].ident);
    //                     delete handler;
    //                 } catch (std::ios_base::failure& f) {
    //                     DEBUG_MSG(ERR_PARSE, R);
    //                 } catch (std::exception& error) {
    //                     EV_SET(&changeList, eventList[i].ident, EVFILT_READ, EV_DELETE,
    //                     0, 0,
    //                         0);
    //                     if (kevent(kq, &changeList, 1, NULL, 0, NULL) == -1) {
    //                         close(kq);
    //                         THROW_EXCEPTION_WITH_INFO(strerror(errno));
    //                     }
    //                     DEBUG_MSG(error.what(), R);
    //                 }
    //             }
    //         }
    //     }

    // close(kq);
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
