#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>

// TODO
// [ ] split-up function implementations into their own files
// [ ] error checking functions for domain, type, protocol ... etc
// [ ] custom exception class for error handling
// [ ] better error messages
// [ ] make a unit-tester main
// [ ] add shutdown functionality (block recv, block send, close())

#ifndef SOCKET_HPP
#define SOCKET_HPP

// error messages
#define ERR_NULL_MSG "Null ptr!!" // make more verbose
#define ERR_STRT_MSG "Socket creation failed"
#define ERR_BIND_MSG "Socket binding failed"
#define ERR_LIST_MSG "Socket binding failed"

// error codes
enum WS_CODE
{
    WS_OK = 0,
    WS_LISTEN_FAIL,
    WS_BIND_FAIL,
    WS_TOTAL
};

// socket_descriptor type
typedef int file_descriptor;

class Socket
{

    public:
        // constructors
        Socket(int domain, int type, int protocol = 0, int flags = SOCK_NONBLOCK);
        Socket(int         domain,
               int         type,
               const char *protocol_name,
               int         flags = SOCK_NONBLOCK);

        // destructor
        ~Socket(); // close vs shutdown

        // member functions (API)
        WS_CODE bind(int port);
        // WS_CODE connect();
        WS_CODE listen(int backlog);
        // WS_CODE accept();
        // WS_CODE send(const char *msg);
        // WS_CODE recv(const char *buff);
        // WS_CODE getpeer(struct sockaddr &address);
        // WS_CODE shutdown(int option);

    private:
        static const int invalid_file_descriptor = -1;
        file_descriptor  socket_descriptor;

        int family; // domain of communicatoin
        int type;   // type of socket

        // deleted but can't cause is 98 maguy
        Socket(const Socket &){};
        Socket &operator=(const Socket &)
        {
            return *this;
        };
};

#endif // SOCKET_HPP
