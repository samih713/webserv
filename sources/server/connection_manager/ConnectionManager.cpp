#include "ConnectionManager.hpp"

/**
 * Removes expired connections from the connection map.
 *
 * @param currentSockets The set of file descriptors representing the current active
 * sockets.
 *
 * @return void
 *
 * @throws None
 */
void ConnectionManager::remove_expired(fd_set& currentSockets)
{
    ConnectionMap::iterator it = connectionMap.begin();
    while (it != connectionMap.end())
        if (it->second.timer.is_timeout()) {
            DEBUG_MSG("Connection closed after select", L);
            FD_CLR(it->first, &currentSockets);
            close(it->first);
            connectionMap.erase(it++);
        }
        else
            it++;
}

/**
 * @brief checks if the connection in the connection map has timed out. If the
 * connection has timed out, an exception is thrown with information about the timeout.
 *
 * @param currentSocket The file descriptor of the socket to check the connection status
 * for.
 * @throws ConnectionTimeoutException if the connection has timed out.
 */
void ConnectionManager::check_connection(fd currentSocket)
{
    ConnectionMap::iterator it = connectionMap.find(currentSocket);
    if (it != connectionMap.end() && it->second.timer.is_timeout())
        THROW_EXCEPTION_WITH_INFO(L "Connection Timed out" RE);
}
