#include "webserv.hpp"

#ifndef SOCKET_HPP
#define SOCKET_HPP

/**
 * Set the socket flag for non-blocking I/O operations
 * based on the operating system.
 */
#if defined(__LINUX__) // SOCK_FLAG
static const int SOCK_FLAG = SOCK_NONBLOCK;
#elif defined(__MAC__)
#include <fcntl.h>
static const int SOCK_FLAG = O_NONBLOCK;
#else
static const int SOCK_FLAG = 0;
#endif

/**
 * @class Socket
 * @brief A class representing a socket for network communication
 *
 * This class provides functionality for creating and managing network sockets.
 */
class Socket
{

    public:
        virtual ~Socket() throw();
        /* [INTERFACE] */
        void set_port(int port);
        fd   get_fd() const throw();
        void bind() const;
        void listen(int backlog) const;
        fd   accept();
        // why is this public

        class Exception : public std::exception
        {

            public:
                explicit Exception(const std::string &error_message);
                ~Exception() throw(){};
                const char *what() const throw();

            private:
                std::string error_message;
                std::string compose_msg(const std::string &message);
        };

    protected:
        Socket(int family, int type, int protocol, int flags);
        struct sockaddr address;

    private:
        static const fd invalidFD = -1;
        fd              socketFD;
        mutable bool    isBound;
        mutable bool    isListening;

        // deleted
        Socket(const Socket &){};
        Socket &operator=(const Socket &)
        {
            return *this;
        };
};

#endif // SOCKET_HPP
