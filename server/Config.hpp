#include "webserv.hpp"

#ifndef CONFIG_HPP
#define CONFIG_HPP

class Config {
public:
    Config(string serverRoot = "../resources/sample_pages",
           string home = "/index.html",
           string notFoundPage = "/404.html",
           fd     listenerPort = 8080)
        : serverRoot(serverRoot)
        , home(serverRoot + home)
        , notFoundPage(serverRoot + notFoundPage)
        , listenerPort(listenerPort)
    {}

    string serverRoot;
    string home;
    string notFoundPage;
    fd     listenerPort;
    vector<string> serverName;
    vector<string> locations;

    void print(void) {
        std::cout << "Server root: " << serverRoot << std::endl;
        std::cout << "Home: " << home << std::endl;
        std::cout << "Not found page: " << notFoundPage << std::endl;
        std::cout << "Listener port: " << listenerPort << std::endl;
    }

};

#endif // CONFIG_HPP
