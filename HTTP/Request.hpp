#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Message.hpp"
#include <string>

namespace webserv {
namespace HTTP {

// context class to hold the message and status

class Request
{

    public:
        // constructor
        Request(const std::string &message);
        // destructor
        ~Request();
        // copy
        Request(const Request &);

        // member functions
        const std::string &get_raw_request() const;
        void               parse();

    private:
        std::string rawRequest;
        HTTP::METHOD      method;
		std::string path;
		std::string version;
};
}; // namespace HTTP
}; // namespace webserv

#endif // REQUEST_HPP
