#ifndef SERVER_HPP
#define SERVER_HPP

// group up in errors struct or enum something
#include <vector>
#define SOCKET_ERR "failed to initiate listener"

// simple singleton implementation
class Server
{

    protected:
        Server();
        Server(const Server &);
        Server &operator=(const Server &);

    public:
        // this function needs to be static, as there won't be an instance of a Server
        // when its first created
        static Server &getInstance();
        void           start(); // !! work on start
        ~Server();

    private:
        int              listener;
        int              nConn;
        std::vector<int> conn;
};

#endif // SERVER_HPP
