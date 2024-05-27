#include "CachedPages.hpp"
#include "ConnectionManager.hpp"
#include "DeleteRequestHandler.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "Logger.hpp"
#include "PostRequestHandler.hpp"
#include "Request.hpp"
#include "ServerConfig.hpp"
#include "TCPSocket.hpp"
#include "error_pages.hpp"
#include "webserv.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP

enum polling_strat {
    KQUEUE,
    SELECT,
    POLL,
    EPOLL
};

// wait message
static const string WAIT_MESSAGE("*** Server is now waiting for connections ***");
// default backLog
static const int DEFAULT_BACKLOG(16);
// default select wait
static const int SELECTWAITTIME(5);
// default kqueue wait
static const int KQUEUEWAITTIME(5);
static const int MAX_EVENTS(16);

class Server {
public:
    static Server& get_instance(ServerConfig& config, int backLog = DEFAULT_BACKLOG);
    ~Server();
    void start(enum polling_strat);

protected:
    Server(ServerConfig& config, int backLog);

private:
    TCPSocket     _listener;
    ServerConfig& _config;
    CachedPages*  _cachedPages;

    IRequestHandler* MakeRequestHandler(const string& method)
    {
        if (method == "GET")
            return new GetRequestHandler;
        else if (method == "POST")
            return new PostRequestHandler;
        else if (method == "DELETE")
            return new DeleteRequestHandler;
        else //! return 501 Not implemented
            THROW_EXCEPTION_WITH_INFO("Request Method not implemented\n");
    }

    void handle_connection(fd incoming, fd_set& activeSockets);

    /* polling strats */
    void select_strat();
    void kqueue_strat();
    // void poll_strat();
    // void epoll_strat();

    // deleted
    Server(const Server&);
    Server& operator=(const Server&);
};

#endif // SERVER_HPP
