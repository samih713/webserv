#include "ServerConfig.hpp"
#include "webserv.hpp"

#ifndef CACHED_PAGES_HPP
#define CACHED_PAGES_HPP

struct Page {
    vector<char> data;
    string       contentType;
    size_t       contentLength;
};

class CachedPages {
public:
    CachedPages(const ServerConfig& config)
    {
        // TODO: load pages in a more dynamic way since rn this is hardcoded
        //  load the error page
        ifstream notFoundDefault(config.errorPages.at(NOT_FOUND).c_str(),
            std::ios_base::binary);
        if (notFoundDefault.fail()) {
            DEBUG_MSG("404 page failed to load", B);
        }
        else {
            /* determine content length */
            notFoundDefault.seekg(0, std::ios_base::end);
            notFound.contentLength = notFoundDefault.tellg();
            notFound.contentType   = "text/html";
            notFoundDefault.seekg(0, std::ios_base::beg);
            notFound.data =
                vector<char>((std::istreambuf_iterator<char>(notFoundDefault)),
                    std::istreambuf_iterator<char>());
        }
        // load the indexDefault page
        ifstream indexDefault(config.indexFile.c_str(), std::ios_base::binary);
        if (indexDefault.fail()) {
            DEBUG_MSG("Index page failed to load", B);
        }
        else {
            /* determine content length */
            indexDefault.seekg(0, std::ios_base::end);
            home.contentLength = indexDefault.tellg();
            indexDefault.seekg(0, std::ios_base::beg);
            home.contentType = "text/html";
            home.data = vector<char>((std::istreambuf_iterator<char>(indexDefault)),
                std::istreambuf_iterator<char>());
        }
    }
    ~CachedPages() { DEBUG_MSG("Cached Pages destructor called", R); }
    vector<Page> pages;
    Page         notFound;
    Page         home;
};

#endif // CACHED_PAGES_HPP
