#include "webserv.hpp"

#ifndef HEADER_HPP
#define HEADER_HPP

static const int DEFAULT_MAX_BODY_SIZE(1048576);

// expected content length
static const int NOT_SET(-1);
static const int NOT_SPECIFIED(0);

enum HEADER_STATE {
    NOT_READY,
    READY_TO_PARSE,
};

struct Header {
    Header();
    ~Header();
    Header(const Header&);
    Header& operator=(const Header&);
    bool    operator()(HEADER_STATE s);

    // API
    void parse_header(istringstream& message);
    // members
    string    method;
    string    resource;
    string    version;
    string    cgiResource;
    int       bodySize;
    HeaderMap fields;

    // states
    HEADER_STATE state;
    bool         chunked;

private:
    void parse_request_line(istringstream& message);
    void add_header(istringstream& message);
    void process_content();
    void is_chunked(const HeaderMap::const_iterator it);
    void find_content_length(HeaderMap::const_iterator it);
};

// errors
static const string ERR_CONTENT_LENGTH("Request_parser: invalid body size");

#endif // HEADER_HPP
