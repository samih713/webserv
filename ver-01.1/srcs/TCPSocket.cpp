#include "TCPSocket.hpp"

TCPSocket::TCPSocket()
    : Socket(AF_INET, SOCK_STREAM, 0, SOCK_NONBLOCK)
{}

TCPSocket::~TCPSocket()
{
    // destructor code
}
