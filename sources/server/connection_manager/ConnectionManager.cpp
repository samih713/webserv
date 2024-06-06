#include "ConnectionManager.hpp"

/**
 * Removes expired connections from the connection map.
 *
 * @return void
 */
void ConnectionManager::remove_expired()
{
    ConnectionMap::iterator it = connectionMap.begin();
    while (it != connectionMap.end())
        if (it->second.timer.is_timeout()) {
            FD_CLR(it->first, &activeSockets);
            close(it->first);
            connectionMap.erase(it++);
            LOG_INFO("Server: connection timed out.");
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
        throw TimeOut::Exception();
}
