#include "ConnectionManager.hpp"

void ConnectionManager::remove_expired(fd_set &currentSockets)
{
    ConnectionMap::iterator it = connectionMap.begin();
    // this has to be a while loop since we are erasing
    while (it != connectionMap.end())
    {
        if (it->second.is_timeout())
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
