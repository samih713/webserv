#include "../includes/Server.hpp"

int main()
{
    Server &webserv = Server::getInstance(80, 10);
    (void)webserv;
}
