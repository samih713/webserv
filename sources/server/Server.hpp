#include "CachedPages.hpp"
#include "ServerConfig.hpp"
#include "ConnectionManager.hpp"
#include "TCPSocket.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP

enum polling_strat {
    KQUEUE,
    SELECT,
    POLL,
    EPOLL
};

// wait message
static const std::string WAIT_MESSAGE("*** Server is now waiting for connections ***");
// default backLog
static const int DEFAULT_BACKLOG(16);
// default select wait
static const int SELECTWAITTIME(5);
// default kqueue wait
static const int KQUEUEWAITTIME(5);
static const int MAX_EVENTS(16);

class Server {
public:
    static Server& get_instance(const ServerConfig& config,
        int                                         backLog = DEFAULT_BACKLOG);
    ~Server();
    void start(enum polling_strat);

protected:
    Server(const ServerConfig& config, int backLog);

private:
    TCPSocket           listener;
    const ServerConfig& config;
    CachedPages*        cachedPages;

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
