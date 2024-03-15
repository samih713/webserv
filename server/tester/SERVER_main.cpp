#include "Server.hpp"
#include <iostream>

int main()
{
    // default constructor of config
    // Config(string serverRoot = "../resources/sample_pages/",
    // string home = "index.html",
    // string notFoundPage = "404.html",
    // fd     listenerPort = 8080)
    //
    Config config;
    try
    {
        Server &webserv = Server::get_instance(config, 10);
        webserv.start(SELECT);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
}
