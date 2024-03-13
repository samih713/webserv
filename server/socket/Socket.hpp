#include "webserv.hpp"
#include <cerrno>
#include <cstring>
#include <exception>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#ifndef SOCKET_HPP
#define SOCKET_HPP

#if defined(__LINUX__) // SOCK_FLAG
static const int SOCK_FLAG = SOCK_NONBLOCK;
#elif defined(__MAC__)
#include <fcntl.h>
static const int SOCK_FLAG = O_NONBLOCK;
#else
static const int SOCK_FLAG = 0;
#endif

class Socket
{
    protected:
        Socket(int family, int type, int protocol, int flags);
        struct sockaddr address;

    public:
        virtual ~Socket();
        /* [INTERFACE] */
        void set_port(int port);
        fd   get_fd() const throw();
        void bind() const;
        void listen(int backlog) const;
        fd   accept();
        // void shutdown(int option);

        struct Exception : public std::exception
        {
            private:
                std::string error_message;

            public:
                explicit Exception(const std::string &error_message)
                    : error_message(error_message){};
                ~Exception() throw(){};
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
        fd socket_descriptor;

    private:
        static const int invalid_file_descriptor = -1;
        mutable bool     is_bound;
        mutable bool     is_listening;
        // deleted
        Socket(const Socket &){};
        Socket &operator=(const Socket &)
        {
            return *this;
        };
};

#endif // SOCKET_HPP
