#include "../Message.hpp"
#include "../TimeOut.hpp"

#ifndef REQUEST_HPP
#define REQUEST_HPP

static const int BUFFER_SIZE(40);

// should be mask bits
static const int NOT_SET(-1);
static const int NOT_SPECIFIED(0);

enum CONNECTION_STATUS
{
    CLOSE_CONNECTION = 0,
    KEEP_ALIVE = 1,
    CONNECTION_ERROR = 2
};

// Request class to parse an incoming message
class Request
{
    public:
        Request();
        ~Request();
        Request(const Request &other);

        CONNECTION_STATUS recv(fd socket);

        METHOD        get_method() const;
        const string &get_resource() const;
        const vsp    &get_headers() const;

        bool parse();

        bool isCompleted();
        void setCompleted();

        TimeOut timer;

        friend ostream &operator<<(ostream &os, const Request &r);

    private:
        void   parse_body();
        void   parse_header();
        void   parse_content_length(const string &contentLength);
        string rawRequest;

        size_t headerEnd;
        bool   headerReady;
        bool   parsed;
        bool   completed;
        int    expectedBodySize;


        METHOD method;
        string resource;
        string http_version;

        vsp         header_fields;
        vsp         trailer_fields;
        std::string body;

        // deleted copy assigment
        void operator=(const Request &);
};

#endif // REQUEST_HPP
