#include "TCPSocket.hpp"

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
        static Server &get_instance(fd listenerPort, int backLog);
        ~Server();
        void start(enum polling_strat);
        void handle_connection(fd recvSocket);

    protected:
        Server(fd listenerPort, int backLog);

    private:
        TCPSocket  listener;
        vector<fd> connections;
        fd         listenerFd;
        int        listenerPort;
        // typedef struct _Config{
		// } Config;
        void select_strat();
        // void kqueue_strat();
        // void poll_strat();
        // void epoll_strat();

        // deleted
        Server(const Server &){};
        Server &operator=(const Server &)
        {
            return *this;
        };
};

#endif // SERVER_HPP
