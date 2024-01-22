#include <cerrno>
#include <cstring>
#include <exception>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "../../includes/error.hpp"

// TODO // [P]artially implemented, needs [I]mprovement, [X] done
//
// [I] make a unit-tester main
// [ ] add shutdown functionality (block recv, block send, close())
// [ ] fix the get_fd functionality is it really necessary
// [ ] split-up function implementations into their own files

#ifndef SOCKET_HPP
#define SOCKET_HPP

// SOCK_FLAG
// prefer the compiler over pre-processor
#if defined(__LINUX__)
static const int SOCK_FLAG = SOCK_NONBLOCK;
#elif defined(__MAC__)
#include <fcntl.h>
static const int SOCK_FLAG = O_NONBLOCK;
#else
static const int SOCK_FLAG = 0;
#endif

// socket_descriptor type
typedef int file_descriptor;

class Socket
{
    protected: // protected constructors only in derived classes
        /*  [CONSTRUCTORS] */

        Socket(int family, int type, int protocol, int flags);
        // maybe not necessary
        Socket(int family, int type, const char *protocol_name, int flags);
        // only used by inherited class
        struct sockaddr address; // type casted and set based on type of socket

    public:
        /* [DESTRUCTOR] */

        virtual ~Socket();

        /* [INTERFACE] */

        void set_port(int port);
        file_descriptor get_fd() const throw();
        void bind() const;
        void listen(int backlog) const;
        file_descriptor accept();
        // void shutdown(int option);
        // void connect(); // is something you do on a client

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
                    _msg << message
                         << (errno ? ": " + std::string(std::strerror(errno)) : "");
                    return _msg.str();
                }
                const char *what() const throw()
                {
                    return error_message.c_str();
                }
        };

        file_descriptor socket_descriptor;

    private:
        static const int invalid_file_descriptor = -1;

        // state checks
        mutable bool is_bound;
        mutable bool is_listening;

        // deleted but can't cause is 98 maguy
        Socket(const Socket &) {};
        Socket &operator=(const Socket &) { return *this; };
};

#endif // SOCKET_HPP
