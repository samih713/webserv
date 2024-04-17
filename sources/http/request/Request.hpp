#include "../Message.hpp"
#include "../TimeOut.hpp"
#include "../../server/ServerConfig.hpp"

#ifndef REQUEST_HPP
#define REQUEST_HPP

static const int BUFFER_SIZE(4096);

// should be mask bits
static const int NOT_SET(-1);
static const int NOT_SPECIFIED(0);

static const string ERR_PARSE("Request_parser: failed to parse request");

// Request class to parse an incoming message
class Request {
public:
    Request(const ServerConfig& config);
    ~Request();
    Request(const Request& other);

    void recv(fd socket);

    METHOD        get_method() const;
    const string& get_resource() const;
    const vsp&    get_headers() const;

    bool parse_request();

    bool isCompleted();
    void setCompleted();

    TimeOut timer;

    friend ostream& operator<<(ostream& os, const Request& r);

private:
    void parse_body();
    void parse_header();
    void parse_content_length(const string& contentLength);

    stringstream message;
    const ServerConfig& config;

    bool headerReady;
    bool parsed;
    bool completed;
    int  expectedBodySize;

    METHOD method;
    string resource;
    string http_version;

    vsp         header_fields;
    vsp         trailer_fields;
    std::string body;

    // deleted copy assigment
    void operator=(const Request&);
};

#endif // REQUEST_HPP
