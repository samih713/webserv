#include "GetRequestHandler.hpp"
#include "Logger.hpp"
#include "webserv.hpp"
#include <dirent.h>

struct DirEntry {
    string name;
    string modTime;
    off_t  size;
    bool   isDir;
};

static bool compare_entries(const DirEntry& a, const DirEntry& b)
{
    if (a.isDir != b.isDir)
        return a.isDir > b.isDir; // Directories first
    return a.name < b.name;       // Alphabetical order
}

vector<char> GetRequestHandler::list_directory(const string& dirPath, const string& uri)
{
    DIR* directory = opendir(dirPath.c_str());
    if (directory == NULL)
        return cp.get_error_page(INTERNAL_SERVER_ERROR).data;

    stringstream ss;
    ss << "<html" << "<head><title>Index of " << uri << "</title></head>\n"
       << "<body>\n"
       << "<h1>Index of " << uri << "</h1><hr><pre><a href=\"../\">../</a>\n";

    struct dirent*   dirEntry;
    vector<DirEntry> entries;
    while ((dirEntry = readdir(directory)) != NULL) {
        string entryName = dirEntry->d_name;
        if (entryName == "." || entryName == ".." || entryName[0] == '.')
            continue;

        struct stat fileInfo;
        if (stat((dirPath + "/" + entryName).c_str(), &fileInfo) == -1) {
            closedir(directory);
            return cp.get_error_page(INTERNAL_SERVER_ERROR).data;
        }

        DirEntry entry;
        entry.name  = entryName;
        entry.isDir = S_ISDIR(fileInfo.st_mode);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", localtime(&fileInfo.st_mtime));
        entry.modTime = buffer;
        entry.size    = fileInfo.st_size;

        if (entry.isDir)
            entry.name += "/";

        entries.push_back(entry);
    }
    closedir(directory);

    // Sort entries by directory status and then by name
    std::sort(entries.begin(), entries.end(), compare_entries);

    for (vector<DirEntry>::iterator entry = entries.begin(); entry != entries.end();
         entry++)
    {
        ss << "<a href=\"" << entry->name << "\">" << entry->name << "</a>"
           << std::setw(70 - entry->name.size()) << std::right << entry->modTime
           << std::setw(24) << std::right << (entry->isDir ? "-" : ws_itoa(entry->size))
           << "\n";
    }

    ss << "</pre><hr></body>\n"
       << "</html>\n";

    LOG_INFO("Server: Serving directory listing");
    string htmlContent = ss.str();
    return vector<char>(htmlContent.begin(), htmlContent.end());
}
