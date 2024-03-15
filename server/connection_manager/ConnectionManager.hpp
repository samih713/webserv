#include "TimeOut.hpp"
#include "map"
#include "webserv.hpp"

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

typedef std::map<fd, TimeOut> ConnectionMap;

class ConnectionManager
{
    public:
        ConnectionManager(){}; // default initialization
        void        remove_expired(fd_set &currentSockets);
        inline void check_connection(fd currentSocket);
        inline void remove_connection(fd currentSocket, fd_set &currentSockets);
        inline void add_connection(fd currentSocket, fd_set &currentSockets);

    private:
        ConnectionMap connectionMap;
};

inline void ConnectionManager::check_connection(fd currentSocket)
{
    // check if currentSocket is already in map
    std::map<fd, TimeOut>::iterator it = connectionMap.find(currentSocket);
    if (it == connectionMap.end())
        connectionMap.insert(std::make_pair(currentSocket, TimeOut()));
    else // update time
        it->second.update_time();
}

inline void ConnectionManager::remove_connection(fd currentSocket, fd_set &currentSockets)
{
    FD_CLR(currentSocket, &currentSockets);
    close(currentSocket);
    connectionMap.erase(connectionMap.find(currentSocket));
}

inline void ConnectionManager::add_connection(fd clientSocket, fd_set &currentSockets)
{
    FD_SET(clientSocket, &currentSockets);
    connectionMap.insert(std::make_pair(clientSocket, TimeOut()));
}

#endif // CONNECTION_MANAGER_HPP
