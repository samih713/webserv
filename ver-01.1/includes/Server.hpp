#include <vector>

#ifndef SERVER_HPP
#define SERVER_HPP


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
        void           start();
        static WS_CODE bind(int sockfd);
        ~Server();

    private:
        int              listener;
        int              status;
        std::vector<int> conn;
};

#endif // SERVER_HPP
