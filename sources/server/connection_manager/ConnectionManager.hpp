#include "TimeOut.hpp"
#include "webserv.hpp"

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

/**
 * Updates the connection time for a given socket in the ConnectionManager.
 *
 * @param currentSocket The file descriptor of the socket for which the connection time
 * needs to be updated.
 * @return void
 *
 * @throws Assertion error if the socket is not found in the ConnectionManager.
 */
inline void ConnectionManager::update_connection(fd currentSocket)
{
    ConnectionMap::iterator it = connectionMap.find(currentSocket);
    DEBUGASSERT(it != connectionMap.end() && "Connection not found");
    it->second.update_time();
}


/**
 * @brief Removes a connection from the active sockets and closes the socket.
 *
 * @param currentSocket The socket to be removed from the active sockets.
 * @param activeSockets Reference to the set of active sockets.
 *
 * @return void
 */
inline void ConnectionManager::remove_connection(fd currentSocket, fd_set &activeSockets)
{
    FD_CLR(currentSocket, &activeSockets);
    close(currentSocket);
    connectionMap.erase(connectionMap.find(currentSocket));
    DEBUG_MSG("Connection closed", L);
}


/**
 * Adds a new connection to the active sockets set and connection map.
 *
 * @param newConnection The file descriptor of the new connection to be added
 * @param activeSockets Reference to the active sockets set
 *
 * @return void
 */
inline void ConnectionManager::add_connection(fd newConnection, fd_set &activeSockets)
{
    FD_SET(newConnection, &activeSockets);
    connectionMap.insert(std::make_pair(newConnection, TimeOut()));
}


#endif // CONNECTION_MANAGER_HPP
