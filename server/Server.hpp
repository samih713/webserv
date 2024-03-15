#include "CachedPages.hpp"
#include "Config.hpp"
#include "ConnectionManager.hpp"
#include "TCPSocket.hpp"
#include "debug.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP

static const int BUFFER_SIZE = 4096;

#define CLOSE_CONNECTION 1
#define KEEP_ALIVE       0

enum polling_strat
{
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

class Server
{
    public:
        static Server &get_instance(const Config &config, int backLog = DEFAULT_BACKLOG);
        ~Server();
        void start(enum polling_strat);

    protected:
        Server(const Config &config, int backLog);

    private:
        TCPSocket     listener;
        const Config &config;
        CachedPages  *cachedPages;

        bool handle_connection(fd recvSocket);
        /* polling strats */
        void select_strat();
        // void kqueue_strat();
        // void poll_strat();
        // void epoll_strat();

        // deleted
        Server(const Server &);
        Server &operator=(const Server &);
};

#endif // SERVER_HPP
