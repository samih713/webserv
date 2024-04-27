#include "Message.hpp"
#include "Server.hpp"
#include "webserv.hpp"
#include <arpa/inet.h>
#include <unistd.h>

const string crlf("\r\n");
const string version("HTTP/1.1");
// const std::string request("GET /index.html " + version + crlf + crlf);
#define BUFFER_SIZE 4096

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        cerr << "Failed to create socket. errno: " << errno << endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port   = htons(8080); // Connecting to port 8080
    if (inet_pton(AF_INET, "127.0.0.1", &sockaddr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported" << endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
        cerr << "Connection Failed. errno: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    if (send(sockfd, sample_request.c_str(), strlen(sample_request.c_str()), 0) < 0) {
        std::cerr << "Failed to send the message. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    cout << "Message sent to server successfully." << endl;

    // Receive a response back from the server
    char    buffer[BUFFER_SIZE + 1] = { 0 };
    ssize_t bytesReceived           = 0;

    bytesReceived = recv(sockfd, buffer, BUFFER_SIZE, 0);

    if (bytesReceived < 0) {
        cerr << "Failed to receive the message. errno: " << errno << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Server's response: " << string(buffer, bytesReceived) << endl;

    // Close the connection
    close(sockfd);
    return 0;
}
