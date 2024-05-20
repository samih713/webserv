#include "webserv.hpp"
#include <dirent.h>

void autoindex(const string& directoryName)
{
    DIR* directory = opendir(directoryName.c_str());
    if (!directory)
        THROW_EXCEPTION_WITH_INFO("Cannot open directory"); //!
}
