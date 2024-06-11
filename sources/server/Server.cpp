#include "Server.hpp"
#include "ConnectionManager.hpp"

/* ------------------------------- CONSTRUCTOR ------------------------------ */

/**
 * @brief Constructor for the Server class.
 *
 * This constructor initializes a Server object with the provided configuration and
 * backlog.
 *
 * @param cfgs The configurations containing settings for the server.
 * @param backLog The maximum length of the queue of pending connections.
 *
 * @throws Socket::Exception if there is an issue with setting up the listener socket.
 */
Server::Server(vector<ServerConfig>& cfgs, int backLog) : cfgs(cfgs)
{
    for (vector<ServerConfig>::iterator sc = cfgs.begin(); sc != cfgs.end(); sc++) {
        sc->cp = new CachedPages(*sc);

        servers[TCPSocket(sc->port, backLog, sc->host)] = *sc;

        string s_host = inet_ntoa(*(struct in_addr*) &sc->host);
        LOG_INFO("Server created on port [" + s_host + ":" + ws_itoa(sc->port) + "]");
    }
    LOG_DEBUG("Server was created successfully");
}

/* ---------------------------- HANDLE CONNECTION --------------------------- */

static IRequestHandler* make_request_handler(Request& r, ServerConfig& cfg)
{
    vector<string>& methods       = cfg.methods;
    const string&   locationMatch = r.get_location_match();

    if (!locationMatch.empty())
        methods = cfg.locations.at(locationMatch).methods;

    const string& method = r.get_method();
    if (method != "GET" && method != "POST" && method != "DELETE")
        r.set_status(NOT_IMPLEMENTED);
    else if (std::find(methods.begin(), methods.end(), method) == methods.end())
        r.set_status(METHOD_NOT_ALLOWED);

    if (r.get_status() != OK)
        return new ErrorHandler(cfg);
    else if (method == "GET")
        return new GetRequestHandler(cfg);
    else if (method == "POST")
        return new PostRequestHandler(cfg);
    else if (method == "DELETE")
        return new DeleteRequestHandler(cfg);
    else
        return NULL;
}

// TODO this function does not work with kqueue, only select
void Server::handle_connection(fd incoming, ServerConfig& cfg)
{
    LOG_INFO("Server: handling connection for [" + ws_itoa(incoming) + "]");
    try {
        ConnectionManager::check_connection(incoming);

        Request& r = ConnectionManager::add_connection(incoming);
        r.recv(incoming);
        if (!r.process(cfg))
            return;

        IRequestHandler* handler  = make_request_handler(r, cfg);
        Response         response = handler->handle_request(r);

        if (r.cgiStatus == NOT_SET) {
            response.send_response(incoming);
            ConnectionManager::remove_connection(incoming);
        }
        else if (r.cgiStatus == IN_PROCESS) {
            r.cgiStatus = TEMP; // set to temp to avoid double processing
            r.cgiClient = incoming;
            ConnectionManager::add_cgi_connection(r);
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
                tcp = ConnectionManager::get_tcp(currentSocket);
                if (FD_ISSET(currentSocket, &listenerFDs)) {
                    incoming = tcp->accept();
                    ConnectionManager::add_fd_pair(tcp, incoming);
                }
                else
                    incoming = currentSocket;
                handle_connection(incoming, servers.at(*tcp));
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
        case KQUEUE: kqueue_strat(); break;
        // case POLL: poll_strat(); break;
        // case EPOLL: epoll_strat(); break;
        default: THROW_EXCEPTION_WITH_INFO("Invalid strategy"); break;
    }
}
