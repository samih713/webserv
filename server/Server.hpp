#include "TCPSocket.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP

// constants
static const std::string wait_message("Server is now waiting for connections...\n");
// buffer size
static const int BUFFER_SIZE = 4096;
// set fd size only for testing
#if 1
#undef FD_SETSIZE
#define FD_SETSIZE 24
#endif

// different polling strategies
enum polling_strat
{
    KQUEUE,
    SELECT,
    POLL,
    EPOLL
};
typedef struct _Config Config;

class Server
{
    protected:
        Server(fd listener_port, int backlog);

    public:
        static Server &getInstance(fd listener_port, int backlog);
        ~Server();
        void start(enum polling_strat);
        void handle_connection(fd recv_socket);

    private:
        Config    *config;
        TCPSocket  listener;
        vector<fd> connections;
        string     root;
        fd         listener_fd;
        int        listener_port;


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
