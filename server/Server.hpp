#include "TCPSocket.hpp"

#ifndef SERVER_HPP
#define SERVER_HPP

// TODO // [P]artially implemented, needs [I]mprovement, [X] done
//
// [ ] handle the connection
// [ ] first determine if its a read or write
// [ ] if read (set the buffer_size) splite the request into
// [ ] if write send
// [ ] implement different strategies

// constants
static const std::string wait_message("Server is now waiting for connections...\n");
// buffer size
static const int BUFFER_SIZE = 4096;

// different polling strategies
enum polling_strat
{
    KQUEUE, // "KQUEUE"
    SELECT, // "SELECT"
    POLL,   // "POLL"
    EPOLL   // "EPOLL"
};

// simple singleton implementation
class Server
{
    protected:
        Server(fd listener_port, int backlog);

    public:
        // this function needs to be static, there won't be an instance of a Server
        // when its first created
        static Server &getInstance(fd listener_port, int backlog);
        void           start(enum polling_strat);
        ~Server();

        // member functions
        void recv();
        // void send(const char *msg);
        // void getpeer(struct sockaddr &address);

    private:
        TCPSocket       _listener;
        fd              _listenerFD;
        int             _listener_port;
        std::vector<fd> _connections;

        // polling strats
        void __select_strat();

        // deleted
        Server(const Server &){};
        Server &operator=(const Server &)
        {
            return *this;
        };
};

#endif // SERVER_HPP
