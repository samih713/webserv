#include "Logger.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "TCPSocket.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

typedef map<fd, Request>          ConnectionMap;
typedef map<fd, const TCPSocket*> TCPfdMap;

class ConnectionManager {
public:
    static void                    remove_expired();
    static void                    check_connection(fd currentSocket);
    static inline void             remove_connection(fd currentSocket);
    static inline Request&         add_connection(fd newConnection);
    static inline void             add_cgi_connection(Request& r);
    static inline fd_set           add_listeners(const servers_t& servers);
    static inline fd&              get_max_sd();
    static inline fd_set&          get_active_sockets();
    static inline const TCPSocket* get_tcp(fd& socketD);
    static inline void             add_fd_pair(const TCPSocket* tcp, fd newFD);

private:
    static TCPfdMap      TCPMap;
    static ConnectionMap connectionMap;
    static fd_set        activeSockets;
    static fd            maxSD;
    ConnectionManager();
};

inline const TCPSocket* ConnectionManager::get_tcp(fd& socketFD)
{
    TCPfdMap::iterator it = TCPMap.find(socketFD);
    if (it == TCPMap.end())
        LOG_ERROR("get_tcp: " + ws_itoa(socketFD));
    DEBUGASSERT(it != TCPMap.end() && "TCP socket not found");
    return it->second;
}

inline fd_set& ConnectionManager::get_active_sockets()
{
    return activeSockets;
}

inline fd& ConnectionManager::get_max_sd()
{
    return maxSD;
}

inline fd_set ConnectionManager::add_listeners(const servers_t& servers)
{
    fd_set listenerFDs;
    FD_ZERO(&activeSockets);
    FD_ZERO(&listenerFDs);
    for (servers_t::const_iterator s = servers.begin(); s != servers.end(); s++) {
        fd listenerFD = s->first.get_fd();
        FD_SET(listenerFD, &activeSockets);
        FD_SET(listenerFD, &listenerFDs);
        TCPMap.insert(make_pair(listenerFD, &(s->first)));
        maxSD = std::max(maxSD, listenerFD);
        LOG_DEBUG("Server: Added listener " + ws_itoa(listenerFD));
    }
    return listenerFDs;
}

/**
 * @brief Removes a connection from the active sockets and closes the socket.
 *
 * @param currentSocket The socket to be removed from the active sockets.
 *
 * @return void
 */
inline void ConnectionManager::remove_connection(fd currentSocket)
{
    close(currentSocket);
    connectionMap.erase(connectionMap.find(currentSocket));
    FD_CLR(currentSocket, &activeSockets);
    LOG_INFO("Connection closed for [" + ws_itoa(currentSocket) + "]");
}

/**
 * @brief Adds a new connection to the connection manager.
 *
 * This function adds a new connection to the connection manager and updates the active
 * sockets.
 *
 * @param newConnection The file descriptor of the new connection.
 *
 * @return A reference to the Request object associated with the new connection.
 */
inline Request& ConnectionManager::add_connection(fd newConnection)
{
    FD_SET(newConnection, &activeSockets);
    Request& r = connectionMap.insert(make_pair(newConnection, Request())).first->second;
    LOG_DEBUG("ConnectionManager: Added connection [" + ws_itoa(newConnection) + "]");
    maxSD = std::max(maxSD, newConnection);
	if (r.get_status() != REQUEST_TIMEOUT) {
		r.timer.update_time();
	}
    return (r);
}

inline void ConnectionManager::add_fd_pair(const TCPSocket* tcp, fd newFD)
{
    TCPMap.insert(make_pair(newFD, tcp));
}

/**
 * @brief Adds the CGI child's read file descriptor to the connection manager.
 *
 * This function adds the CGI child's read file descriptor to the connection manager and
 * updates the active sockets to let select monitor the CGI child's read file descriptor.
 *
 * @param r The Request object associated with the CGI child.
 *
 * @return void
 */
inline void ConnectionManager::add_cgi_connection(Request& r)
{
    FD_SET(r.cgiReadFd, &activeSockets);
    const TCPSocket* tcp = get_tcp(r.cgiClient);
    connectionMap.insert(make_pair(r.cgiReadFd, r));
    TCPMap.insert(make_pair(r.cgiReadFd, tcp));
    maxSD = std::max(maxSD, r.cgiReadFd);
}

#endif // CONNECTION_MANAGER_HPP
