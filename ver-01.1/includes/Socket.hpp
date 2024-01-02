#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

// TODO
// [ ] error checking functions for domain, type, protocol ... etc
// [ ] custom exception class for error handling
// [ ] better error messages

#ifndef SOCKET_HPP
#define SOCKET_HPP

// error messages
#define SOCKET_ERR "socket failed"

// error codes
enum WS_CODE
{
    WS_OK = 0,
    WS_LISTEN_FAIL,
    WS_BIND_FAIL,
    WS_TOTAL
};

// socket_descriptor type
// cant use alias
typedef int file_descriptor;

// ports
#define PORT_HTTP 80

class Socket
{

    public:
        Socket(int domain, int type, int protocol);
        ~Socket();
        // member functions
        WS_CODE bind(int family, int port, int address = INADDR_ANY);
        WS_CODE listen(int backlog);

    private:
        static const int invalid_file_descriptor = -1;
        file_descriptor  socket_descriptor;

        // deleted but can't cause is 98 maguy
        Socket(const Socket &);
        Socket &operator=(const Socket &);
};

#endif // SOCKET_HPP
