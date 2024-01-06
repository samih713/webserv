#include "TCPSocket.hpp"
#include <Server.hpp>
#include <exception>
#include <iostream>
#include <netinet/in.h>

// Socket tester main add test cases here to test the behaviour of the
// class

#include <sys/select.h>

#if 1
int main()
{
    // make a listening socket
    TCPSocket a;
    Socket   *b = &a;
    // prep it to accept connections on port 8080
    a.set_port(8080); // oh-o !!!
    b->bind();

    // set it to listen with backlog 25
    b->listen(5);

    fd_set read_fds;
    int    fdmax = b->get_fd();

    while (true)
    {

        std::cout << "---------- Server is up and waiting!!! ----------------"
                  << std::endl;
        FD_ZERO(&read_fds);
        FD_SET(b->get_fd(), &read_fds);

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Error with select" << std::endl;
            continue;
        }

        if (FD_ISSET(b->get_fd(), &read_fds))
        {
            // accept if a connection happens
            file_descriptor peer = b->accept();
            std::cout << "peer is connected on port " << peer << "\n";

            // receive data from the connected peer
            char    buffer[1024];
            ssize_t bytesRead = recv(peer, buffer, sizeof(buffer), 0);
            if (bytesRead == -1)
            {
                std::cerr << "Error receiving data from peer" << std::endl;
                continue;
            }

            // process the received data
            std::string receivedData(buffer, bytesRead);
            std::cout << "Received data from peer: " << receivedData << std::endl;

            // send a response back to the peer
            std::string response = "Hello from the server!";
            ssize_t     bytesSent = send(peer, response.c_str(), response.length(), 0);
            if (bytesSent == -1)
            {
                std::cerr << "Error sending";
            }
        }
    }
}
#endif // TCPSocket
