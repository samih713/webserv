#include "Message.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(8080); // Connecting to port 8080
    if (inet_pton(AF_INET, "127.0.0.1", &sockaddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cerr << "Connection Failed. errno: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (send(sockfd, sample_request.c_str(), strlen(sample_request.c_str()), 0) < 0)
    {
        std::cerr << "Failed to send the message. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Message sent to server successfully." << std::endl;

    // Receive a response back from the server
    char    buffer[1024] = { 0 };
    ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0)
    {
        std::cerr << "Failed to receive the message. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server's response: " << std::string(buffer, bytesReceived) << std::endl;

    // Close the connection
    close(sockfd);
    return 0;
}
