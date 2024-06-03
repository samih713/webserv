#include "Message.hpp"
#include "Server.hpp"
#include "webserv.hpp"
#include <arpa/inet.h>
#include <unistd.h>

const string crlf("\r\n");
const string request("GET /index.html " + HTTP_VER + crlf + crlf);
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

    int sent(0);
    int totalSent(0);
    int lengthTosend = strlen(sample_request.c_str());
    while (totalSent < lengthTosend) {
        sent = send(sockfd, sample_request.substr(totalSent).c_str(),
            lengthTosend - totalSent, 0);
        if (sent == -1) {
            cerr << "Failed to send the message. errno: " << errno << endl;
            exit(EXIT_FAILURE);
        }
        totalSent += sent;
    }
    cout << "Message sent to server successfully." << endl;

    // Receive a response back from the server
    string response;
    char   buffer[BUFFER_SIZE + 1] = { 0 };
    int    bytesReceived           = 0;

    while (true) {
        bytesReceived = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytesReceived <= 0)
            break;
        response.append(buffer, bytesReceived);
    }

    cout << "Server's response: " << response << endl;

    // Close the connection
    close(sockfd);
    return 0;
}
