#include "./TimeOut.hpp"
#include "../../../includes/webserv.hpp"

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

typedef std::map<fd, TimeOut> ConnectionMap;

class ConnectionManager
{
    public:
        static void        remove_expired(fd_set &activeSockets);
        static inline void update_connection(fd currentSocket);
        static inline void remove_connection(fd currentSocket, fd_set &activeSockets);
        static inline void add_connection(fd newConnection, fd_set &activeSockets);

    private:
        static ConnectionMap connectionMap;
        ConnectionManager();
};

inline void ConnectionManager::update_connection(fd currentSocket)
{
    ConnectionMap::iterator it = connectionMap.find(currentSocket);
	DEBUGASSERT(it != connectionMap.end() && "Connection not found");
	it->second.update_time();
}

inline void ConnectionManager::remove_connection(fd currentSocket, fd_set &activeSockets)
{
    FD_CLR(currentSocket, &activeSockets);
    close(currentSocket);
    connectionMap.erase(connectionMap.find(currentSocket));
	DEBUG_MSG("Connection closed", L);
}

inline void ConnectionManager::add_connection(fd newConnection, fd_set &activeSockets)
{
    FD_SET(newConnection, &activeSockets);
    connectionMap.insert(std::make_pair(newConnection, TimeOut()));
}

#endif // CONNECTION_MANAGER_HPP
