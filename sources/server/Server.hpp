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
// default max events to wait on
static const int MAX_EVENTS(16);

class Server {
public:
    static Server& get_instance(ServerConfig& cfg, int backLog = DEFAULT_BACKLOG);
    ~Server();
    void start(enum polling_strat);

private:
    TCPSocket     listener;
    ServerConfig& cfg;
    CachedPages*  cp;

    Server(ServerConfig& cfg, int backLog);

    IRequestHandler* make_request_handler(Request& r)
    {
        vector<string>& methods       = cfg.methods;
        const string&   locationMatch = r.get_location_match();

        if (!locationMatch.empty())
            methods = cfg.locations.at(locationMatch).methods;

        const string& method = r.get_method();
        if (method != "GET" && method != "POST" && method != "DELETE")
            THROW_EXCEPTION_WITH_INFO("Request: Method not implemented\n"); //! 501
        else if (std::find(methods.begin(), methods.end(), method) == methods.end())
            THROW_EXCEPTION_WITH_INFO("Request: Method not allowed\n"); //! 405

        if (method == "GET")
            return new GetRequestHandler(cfg, *cp);
        else if (method == "POST")
            return new PostRequestHandler(cfg, *cp);
        else if (method == "DELETE")
            return new DeleteRequestHandler(cfg, *cp);
        else
            return NULL;
    }

    void handle_connection(fd incoming, fd_set& activeSockets, fd& maxSocketDescriptor);

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
