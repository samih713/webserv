#include "webserv.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

class Config
{
    public:
        Config(string serverRoot = "../resources/sample_pages/",
               string defaultPage = "index.html",
               string notFoundPage = "404.html",
               fd     listenerPort = 8080)
            : serverRoot(serverRoot)
            , defaultPage(serverRoot + defaultPage)
            , notFoundPage(serverRoot + notFoundPage)
            , listenerPort(listenerPort)
        {}
        string serverRoot;
        string defaultPage;
        string notFoundPage;
        fd     listenerPort;
};

#endif // CONFIG_HPP
