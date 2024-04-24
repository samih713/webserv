#include "Header.hpp"
#include "Message.hpp"
#include "ServerConfig.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef REQUEST_HPP
#define REQUEST_HPP

static const int BUFFER_SIZE(4096);

class Request {
public:
    Request();
    ~Request();
    Request(const Request& other);

    void recv(fd socket);
    bool process(const ServerConfig& config);

    STATUS_CODE      get_status() const;
    const string&    get_method() const;
    const string&    get_resource() const;
    const HeaderMap& get_headers() const;

    void set_status(STATUS_CODE);

    TimeOut timer;

    friend ostream& operator<<(ostream& os, const Request& r);

private:
    bool parse_body();
    void apply_config(const ServerConfig& config);

    STATUS_CODE status;
    Header      header;

    stringstream message;

    int recievedBodyLength;

    string body;

    // deleted copy assigment
    void operator=(const Request&);
};

// errors
static const string ERR_PARSE("Request_parser: failed to parse request");

#endif // REQUEST_HPP
