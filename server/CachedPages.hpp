#include "Config.hpp"
#include "webserv.hpp"

#ifndef CACHED_PAGES_HPP
#define CACHED_PAGES_HPP

struct Page
{
        vector<char> data;
        string       contentType;
        size_t       contentLength;
};

class CachedPages
{
    public:
        CachedPages(const Config &config)
        {
            // load the error page
            ifstream error_page(config.notFoundPage.c_str(), std::ios_base::binary);
            if (error_page.fail())
            {
                DEBUG_MSG("404 page failed to load", B);
            }
            else
            {
                /* determine content length */
                error_page.seekg(0, std::ios_base::end);
                notFound.contentLength = error_page.tellg();
                notFound.contentType = "text/html";
                error_page.seekg(0, std::ios_base::beg);

                notFound.data = vector<char>((std::istreambuf_iterator<char>(error_page)),
                                             std::istreambuf_iterator<char>());
            }
            // load the index page
            ifstream index_page(config.defaultPage.c_str(), std::ios_base::binary);
            if (index_page.fail())
            {
                DEBUG_MSG("Index page failed to load", B);
            }
            else
            {
                /* determine content length */
                index_page.seekg(0, std::ios_base::end);
                defaultPage.contentLength = error_page.tellg();
                index_page.seekg(0, std::ios_base::beg);
                defaultPage.contentType = "text/html";
                defaultPage.data =
                    vector<char>((std::istreambuf_iterator<char>(index_page)),
                                 std::istreambuf_iterator<char>());
            }
        }
        ~CachedPages()
        {
            DEBUG_MSG("Cached Pages destructor called", R);
        }
        Page notFound;
        Page defaultPage;
};

#endif // CACHED_PAGES_HPP
