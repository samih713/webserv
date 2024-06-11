#include "Server.hpp"
#include <climits>
#include <iostream>
#include <netinet/in.h>
#include <sys/select.h>


// Socket tester main add test cases here to test the behaviour of the
// simple server
int main()
{
    // make a listening socket
    TCPSocket a(8080, 16, htonl(INADDR_ANY));
    // prep it to accept connections on port 8080
    a.set_port(8080); // oh-o !!!
    a.bind();

    // set it to listen with backlog 25
    a.listen(25);
    fd_set read_fds;
    int    fdmax = a.get_fd();

    while (true) {
        cout << "---------- Server is up and waiting!!! ----------------" << endl;
        FD_ZERO(&read_fds);
        FD_SET(a.get_fd(), &read_fds);

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            cerr << "Error with select" << endl;
            continue;
        }

        if (FD_ISSET(a.get_fd(), &read_fds)) {
            // accept if a connection happens
            fd peer = a.accept();
            cout << "peer is connected on port " << peer << "\n";

            // receive data from the connected peer
            char    buffer[1024];
            ssize_t bytesRead = recv(peer, buffer, sizeof(buffer), 0);
            if (bytesRead == -1) {
                cerr << "Error receiving data from peer" << endl;
                continue;
            }

            // process the received data
            string receivedData(buffer, bytesRead);
            cout << "Received data from peer: " << receivedData << endl;

            // send a response back to the peer
            string  response  = "Hello from the server!";
            ssize_t bytesSent = send(peer, response.c_str(), response.length(), 0);
            if (bytesSent == -1)
                cerr << "Error sending";
        }
    }
}
