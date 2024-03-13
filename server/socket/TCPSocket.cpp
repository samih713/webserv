#include "TCPSocket.hpp"

#ifdef __DEBUG__
#include <iostream>
#endif // __DEBUG__

/**
 * @brief Default constructor for TCPSocket
 *
 * This constructor initializes a TCPSocket object with default values.
 * It sets up the socket address structure with INADDR_ANY and family AF_INET.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
TCPSocket::TCPSocket()
    : Socket(family, type, 0, SOCK_FLAG)
{
    struct sockaddr_in addr;
    // prep up the struct
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = family;
    std::memcpy(&address, &addr, sizeof(addr));

    DEBUG_MSG("TCPSocket created successfully", C);
}


/* @brief Destructor for TCPSocket */
TCPSocket::~TCPSocket()
{
    DEBUG_MSG("TCPSocket closed!!", R);
}
