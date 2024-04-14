#include "./ConnectionManager.hpp"

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
void ConnectionManager::remove_expired(fd_set &currentSockets)
{
    ConnectionMap::iterator it = connectionMap.begin();
    while (it != connectionMap.end())
    {
        if (it->second.timer.is_timeout())
        {
            DEBUG_MSG("Connection closed after select", L);
            FD_CLR(it->first, &currentSockets);
            close(it->first);
            connectionMap.erase(it++);
        }
        else
            it++;
    }
}

bool ConnectionManager::check_connection(fd currentSocket)
{
    ConnectionMap::iterator it = connectionMap.find(currentSocket);
    if (it == connectionMap.end())
        return true;
    else if (it->second.timer.is_timeout())
    {
        DEBUG_MSG("Connection Timed out", L);
        return false;
    }
    return true;
}
