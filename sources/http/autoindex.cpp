#include "webserv.hpp"
#include <dirent.h>

/**
 * @brief Building the html file for directory listing to be displayed
 * 
 * @param directoryName name/path of directory to look into
*/
void autoindex(const string& directoryName)
{
    DIR* directory = opendir(directoryName.c_str());
    if (!directory)
        THROW_EXCEPTION_WITH_INFO("Cannot open directory"); //!
}
