#include "Request.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

typedef map<fd, Request> ConnectionMap;

class ConnectionManager {
public:
    static void            remove_expired(fd_set& activeSockets);
    static inline void     update_connection(fd currentSocket);
    static inline Request& find_request(fd currentSocket);
    static void            check_connection(fd currentSocket);
    static inline void     remove_connection(fd currentSocket, fd_set& activeSockets);
    static inline Request& add_connection(fd newConnection, fd_set& activeSockets);

private:
    static ConnectionMap connectionMap;
    ConnectionManager();
};

inline Request& ConnectionManager::find_request(fd currentSocket)
{
    return connectionMap.find(currentSocket)->second;
}

/**
 * Updates the connection time for a given socket in the ConnectionManager.
 *
 * @param currentSocket The file descriptor of the socket for which the connection time
 * needs to be updated.
 * @return void
 */
inline void ConnectionManager::update_connection(fd currentSocket)
{
    ConnectionMap::iterator it = connectionMap.find(currentSocket);
    DEBUGASSERT(it != connectionMap.end() && "Connection not found");
    it->second.timer.update_time();
}


/**
 * @brief Removes a connection from the active sockets and closes the socket.
 *
 * @param currentSocket The socket to be removed from the active sockets.
 * @param activeSockets Reference to the set of active sockets.
 *
 * @return void
 */
inline void ConnectionManager::remove_connection(fd currentSocket, fd_set& activeSockets)
{
    FD_CLR(currentSocket, &activeSockets);
    close(currentSocket);
    connectionMap.erase(connectionMap.find(currentSocket));
    DEBUG_MSG("Connection closed", L);
}


/**
 * @brief Adds a new connection to the connection manager.
 *
 * This function adds a new connection to the connection manager and updates the active
 * sockets.
 *
 * @param newConnection The file descriptor of the new connection.
 * @param activeSockets The set of active sockets.
 * @return A reference to the Request object associated with the new connection.
 */
inline Request& ConnectionManager::add_connection(fd newConnection, fd_set& activeSockets)
{
    FD_SET(newConnection, &activeSockets);
    Request& r = connectionMap.insert(make_pair(newConnection, Request())).first->second;
    r.timer.update_time();
    return (r);
}

#endif // CONNECTION_MANAGER_HPP
