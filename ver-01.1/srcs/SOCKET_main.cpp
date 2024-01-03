#include "Socket.hpp"
#include <exception>
#include <iostream>
#include <netinet/in.h>


int main()
{

    try
    {
        Socket a(PF_INET, SOCK_STREAM, "UDP");
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
}
