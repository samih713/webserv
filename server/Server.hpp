#include "TCPSocket.hpp"
#include <string>
#include <vector>

#ifndef SERVER_HPP
#define SERVER_HPP


static const std::string wait_message("Server is now waiting for connections...\n");

// simple singleton implementation
class Server
{
    protected:
        Server(file_descriptor listenerport, int backlog);

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
        TCPSocket                    _listener;
        int                          _listenerPort;
        std::vector<file_descriptor> _connections;

        // deleted
        Server(const Server &) {};
        Server &operator=(const Server &) { return *this; };
};

#endif // SERVER_HPP
