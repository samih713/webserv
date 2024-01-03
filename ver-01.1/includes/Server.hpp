#include <Socket.hpp>
#include <vector>

#ifndef SERVER_HPP
#define SERVER_HPP


// simple singleton implementation
class Server
{

    protected:
        Server(){};
        Server(const Server &){};
        Server &operator=(const Server &)
        {
            return *this;
        };

    public:
        // this function needs to be static, as there won't be an instance of a Server
        // when its first created
        static Server &getInstance();
        // void           start();
        ~Server();

    private:
        // Socket           listener;
        int              status;
        std::vector<int> conn;
};

#endif // SERVER_HPP
