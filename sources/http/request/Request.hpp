#include "Message.hpp"
#include "ServerConfig.hpp"
#include "TimeOut.hpp"
#include "webserv.hpp"

#ifndef REQUEST_HPP
#define REQUEST_HPP

static const int BUFFER_SIZE(4096);
static const int DEFAULT_MAX_BODY_SIZE(1048576);

// should be mask bits
static const int NOT_SET(-1);
static const int NOT_SPECIFIED(0);
// header states

enum HEADER_STATE {
    NOT_READY,
    READY_TO_PARSE,
    NOT_PARSED,
    PARSED,
};

static const string ERR_PARSE("Request_parser: failed to parse request");

// Request class to parse an incoming message
class Request {
public:
    Request();
    ~Request();
    Request(const Request& other);

    void recv(fd socket);

    STATUS_CODE      get_status() const;
    const string&    get_method() const;
    const string&    get_resource() const;
    const HeaderMap& get_headers() const;

    bool parse_request(const ServerConfig& config);

    bool is_completed();
    void set_completed();
    void set_status(STATUS_CODE);

    TimeOut timer;

    friend ostream& operator<<(ostream& os, const Request& r);

private:
    STATUS_CODE status;
    void        parse_body();
    void        parse_request_line();
    void        parse_header();
    void        parse_header_fields_content();
    void        parse_content_length(HeaderMap::const_iterator it);
    void        apply_config(const ServerConfig& config);

    istringstream message;

    int  headerState;
    bool parsed;
    bool completed;
    int  expectedBodySize;
    int  maxBodySize;

    string method;
    string resource;
    string cgiResource;
    string httpVersion;

    HeaderMap headerFields;
    vsp       trailer_fields;
    string    body;

    // deleted copy assigment
    void operator=(const Request&);
};

#endif // REQUEST_HPP
