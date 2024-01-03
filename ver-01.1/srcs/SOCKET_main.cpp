#include "TCPSocket.hpp"
#include <exception>
#include <iostream>
#include <netinet/in.h>

// Socket tester main add test cases here to test the behaviour of the
// class
int main()
{
    try
    {
        TCPSocket a;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what();
    }
}
