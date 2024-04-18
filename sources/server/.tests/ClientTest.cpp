#include "../../http/Message.hpp"
#include "../socket/TCPSocket.hpp"
#include <arpa/inet.h>
#include <unistd.h>

const string      crlf("\r\n");
const string      version("HTTP/1.1");
const std::string request("GET /index.html " + version + crlf + crlf);
#define BUFFER_SIZE 4096

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port   = htons(8080); // Connecting to port 8080
    if (inet_pton(AF_INET, "127.0.0.1", &sockaddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
        std::cerr << "Connection Failed. errno: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (send(sockfd, request.c_str(), strlen(request.c_str()), 0) < 0) {
        std::cerr << "Failed to send the message. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Message sent to server successfully." << std::endl;

    // Receive a response back from the server
    char    buffer[BUFFER_SIZE] = { 0 };
    ssize_t bytesReceived       = 0;
    ssize_t result              = 0;

    while (1) {
        result = recv(sockfd, &buffer[bytesReceived], 200, 0);
        sleep(1);
        if (result == 0)
            break;
        bytesReceived += result;
    }

    if (bytesReceived < 0) {
        std::cerr << "Failed to receive the message. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server's response: " << std::string(buffer, bytesReceived) << std::endl;

    // Close the connection
    close(sockfd);
    return 0;
}
