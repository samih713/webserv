#include "Header.hpp"
#include "Message.hpp"
#include "ServerConfig.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef REQUEST_HPP
#define REQUEST_HPP

static const int BUFFER_SIZE(4096);

static const int NOT_SET(-1);
static const int COMPLETED(-2);
static const int IN_PROCESS(44);
static const int TEMP(33);

class Request {
public:
    Request();
    ~Request();
    Request(const Request& other);

    void recv(fd socket);
    bool process(const ServerConfig& cfg);

    STATUS_CODE      get_status() const;
    const string&    get_method() const;
    const string&    get_resource() const;
    const string&    get_body() const;
    const HeaderMap& get_headers() const;
    const string&    get_query_string() const;
    const string&    get_resource_path() const;
    const string&    get_location_match() const;

    void set_status(STATUS_CODE);

    TimeOut timer;

    friend ostream& operator<<(ostream& os, const Request& r);

    //! CGI
    mutable int   cgiStatus;
    mutable fd    cgiReadFd;
    fd            cgiClient;
    mutable pid_t cgiChild;

private:
    bool parse_body();
    bool parse_chunked_body();
    void apply_config(const ServerConfig& cfg);

    STATUS_CODE  status;
    Header       header;
    stringstream message;
    string       body;
    string       resourcePath;
    string       locationMatch;

    // deleted copy assigment
    void operator=(const Request&);
};

// errors
static const string ERR_PARSE("Request_parser: failed to parse request");

#endif // REQUEST_HPP
