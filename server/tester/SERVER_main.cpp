#include "Server.hpp"
#include <iostream>

int main()
{
    try
    {
        Server &webserv = Server::getInstance(8080, 10);
        webserv.start(SELECT);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
}
