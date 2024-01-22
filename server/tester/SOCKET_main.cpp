#include "../Server.hpp"
#include "../socket/TCPSocket.hpp"
#include <climits>
#include <iostream>
#include <netinet/in.h>
#include <sys/select.h>


// Socket tester main add test cases here to test the behaviour of the
// class


// simple server
#if 0
int main()
{
    // make a listening socket
    TCPSocket a;
    // prep it to accept connections on port 8080
    a.set_port(8080); // oh-o !!!
    a.bind();

    // set it to listen with backlog 25
	a.listen(25);
    fd_set read_fds;
    int    fdmax = a.get_fd();

    while (true)
    {

        std::cout << "---------- Server is up and waiting!!! ----------------"
                  << std::endl;
        FD_ZERO(&read_fds);
        FD_SET(a.get_fd(), &read_fds);

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Error with select" << std::endl;
            continue;
        }

        if (FD_ISSET(a.get_fd(), &read_fds))
        {
            // accept if a connection happens
            file_descriptor peer = a.accept();
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


// set_port
#if 1

// Define the X macro for test cases
#define TEST_CASES                                                                       \
    X(0)                                                                                 \
    X(-1)                                                                                \
    X(-INT_MAX)                                                                          \
    X(INT_MAX)                                                                           \
    X(INT_MIN)                                                                           \
    X('a')                                                                               \
    X(8080)                                                                              \
    X(99)

int main()
{
#define X(test_case)                                                                     \
    do                                                                                   \
    {                                                                                    \
        std::cout << "---- Case[" << test_case << "] -----\n";                           \
        TCPSocket socket;                                                                \
		try { \
			socket.set_port(test_case);                                                      \
			socket.bind();                                                                   \
		} \
		catch (Socket::Exception &se) { \
			std::cerr << "Error in port number\n"; \
		} \
    } while (false);
    TEST_CASES
#undef X // Clean up the macro definition

    return 0;
}

#endif
