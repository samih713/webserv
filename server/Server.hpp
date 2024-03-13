#include "./socket/TCPSocket.hpp"
#include "CachedPages.hpp"
#include "Config.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP


// different polling strategies
enum polling_strat
{
    KQUEUE,
    SELECT,
    POLL,
    EPOLL
};

class Server
{

    public:
        static Server &get_instance(const Config &config, int backLog = 16);
        ~Server();
        void start(enum polling_strat);
        void handle_connection(fd recvSocket);

    protected:
        Server(const Config &config, int backLog);

    private:
        // config
        const Config &config;

        TCPSocket  listener;
        vector<fd> connections;
        fd         listenerFd;

        // Cached Pages
        CachedPages *cachedPages;

        void select_strat();
        // void kqueue_strat();
        // void poll_strat();
        // void epoll_strat();

        // deleted
        Server(const Server &other)
            : config(other.config){};
        Server &operator=(const Server &)
        {
            return *this;
        };
};

#endif // SERVER_HPP
