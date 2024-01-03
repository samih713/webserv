#include <cerrno>
#include <cstring>
#include <exception>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

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

    protected: // protected constructors only in derived classes
        /*  [CONSTRUCTORS] */

        Socket(int domain, int type, int protocol, int flags);
        // maybe not necessary
        Socket(int domain, int type, const char *protocol_name, int flags);


    public:
        /* [DESTRUCTOR] */
        virtual ~Socket();

        /* [INTERFACE] */

        void bind(int port);
        // void connect();
        void listen(int backlog);
        // void accept();
        // void send(const char *msg);
        // void recv(const char *buff);
        // void getpeer(struct sockaddr &address);
        // void shutdown(int option);

        /* [ERROR HANDLING] */

        struct Exception : public std::exception
        {
            private:
                std::string error_message;

            public:
                explicit Exception(const std::string &error_message)
                    : error_message(error_message){};
                ~Exception() throw(){};

                /* [COMPOSE MESSAGE] */
                static std::string compose_msg(const std::string &message)
                {
                    std::stringstream _msg;
                    _msg << message << (errno ? std::strerror(errno) : "");
                    return _msg.str();
                }
                const char *what() const throw()
                {
                    return error_message.c_str();
                }
        };
        ;

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
