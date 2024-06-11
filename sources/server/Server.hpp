#include "CachedPages.hpp"
#include "DeleteRequestHandler.hpp"
#include "ErrorHandler.hpp"
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

typedef map<TCPSocket, ServerConfig> servers_t;

// default backLog
static const int DEFAULT_BACKLOG(16);
// default select wait
static const int SELECTWAITTIME(5);

class Server {
public:
    static Server& get_instance(vector<ServerConfig>& cfgs, int backLog = DEFAULT_BACKLOG)
    {
        static Server instance(cfgs, backLog);
        return instance;
    }

    ~Server()
    {
        for (servers_t::iterator s = servers.begin(); s != servers.end(); s++)
            delete s->second.cp;
    }

    void start(enum polling_strat);

private:
    servers_t             servers;
    vector<ServerConfig>& cfgs;

    Server(vector<ServerConfig>& cfgs, int backLog);
    void handle_connection(fd incoming, ServerConfig& cfg);

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
