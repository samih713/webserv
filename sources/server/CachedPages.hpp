#include "ServerConfig.hpp"
#include "webserv.hpp"


#ifndef CACHED_PAGES_HPP
#define CACHED_PAGES_HPP

struct Page {
    Page() : contentType("text/html"), contentLength(0) {}

    vector<char> data;
    string       contentType;
    size_t       contentLength;
};

/**
 * pages is a map of page names and their corresponding struct page.
 * ideally, pages should contain:
 // cfg
 *  [index]      = index file data
 *  [error code] = error page
*/
class CachedPages {
public:
    CachedPages(const ServerConfig& cfg);
    ~CachedPages() { DEBUG_MSG("Cached Pages destructor called", R); }

    Page& get_error_page(STATUS_CODE status);
    Page& get_page(const string& pageName);

private:
    //* name, struct Page
    map<string, Page> pages;
    bool              load_page(const string& path, const string& name);
    void              generate_error_page(STATUS_CODE status);
};

#endif // CACHED_PAGES_HPP
