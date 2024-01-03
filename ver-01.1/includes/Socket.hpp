#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>

// TODO // [P]artially implemented, needs [I]mprovement, [X] done
//
// [ ] Derived TCP class to lock domain, type .. combinations
// [ ] split-up function implementations into their own files
// [ ] add shutdown functionality (block recv, block send, close())
// [P] make a unit-tester main
// [X] error checking functions for domain, type, protocol ... etc
// [X] better error messages, and error handling,custom exception class

#ifndef SOCKET_HPP
#define SOCKET_HPP
// DEBUG BUILD
#ifdef __DEBUG__
#include <cassert>
#include <iostream>
#define DEBUGASSERT(x) assert(x)
#else
#define DEBUGASSERT(x)                                                                   \
    do                                                                                   \
    {                                                                                    \
    } while (0)
#endif // __DEBUG__
// DEBUG BUILD

// [ERROR MESSAGES]
static std::string ERR_NULL("Socket: null ptr error");
static std::string ERR_CREAT("Socket: creation failed");
static std::string ERR_BIND("Socket: binding failed");
static std::string ERR_LIST("Socket: listen failed");

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
