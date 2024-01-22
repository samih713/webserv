#include "../Server.hpp"
#include <iostream>

int main()
{
    try
    {
        Server &webserv = Server::getInstance(8080, 10);
        webserv.start();
    }
    catch (const Socket::Exception &e)
    {std::cerr << e.what();}
}
