#include "TCPSocket.hpp"
#include <vector>

#ifndef SERVER_HPP
#define SERVER_HPP


// simple singleton implementation
class Server
{
    protected:
        Server(file_descriptor _listener_port, int _backlog);
        // deleted
        Server(const Server &){};
        Server &operator=(const Server &)
        {
            return *this;
        };

    public:
        // this function needs to be static, as there won't be an instance of a Server
        // when its first created
        static Server &getInstance(file_descriptor listener_port, int backlog);
        void           start();
        ~Server();
        // member functions
        // void recv(const char *buff);
        // void send(const char *msg);
        // void getpeer(struct sockaddr &address);


    private:
        TCPSocket       listener;
        file_descriptor listener_port;
        int             status;
        // std::vector<file_descriptor> conn;
};

#endif // SERVER_HPP
