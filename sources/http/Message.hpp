#include "webserv.hpp"

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

/* ------------------------------ const strings ----------------------------- */
static const string CRLF     = "\r\n";
static const string NEWLINE  = "\n";
static const string SP       = " ";
static const string HTTP_VER = "HTTP/1.1";

/* ---------------------------- STATUS_CODE ENUM ---------------------------- */
#define STATUS_CODE_LIST                                                                 \
    X(CONTINUE)                                                                          \
    X(SWITCHING_PROTOCOLS)                                                               \
    X(PROCESSING)                                                                        \
    X(EARLY_HINTS)                                                                       \
    X(OK)                                                                                \
    X(CREATED)                                                                           \
    X(ACCEPTED)                                                                          \
    X(NON_AUTHORITATIVE_INFORMATION)                                                     \
    X(NO_CONTENT)                                                                        \
    X(RESET_CONTENT)                                                                     \
    X(PARTIAL_CONTENT)                                                                   \
    X(MULTI_STATUS)                                                                      \
    X(ALREADY_REPORTED)                                                                  \
    X(IM_USED)                                                                           \
    X(MULTIPLE_CHOICES)                                                                  \
    X(MOVED_PERMANENTLY)                                                                 \
    X(FOUND)                                                                             \
    X(SEE_OTHER)                                                                         \
    X(NOT_MODIFIED)                                                                      \
    X(USE_PROXY)                                                                         \
    X(TEMPORARY_REDIRECT)                                                                \
    X(PERMANENT_REDIRECT)                                                                \
    X(BAD_REQUEST)                                                                       \
    X(UNAUTHORIZED)                                                                      \
    X(PAYMENT_REQUIRED)                                                                  \
    X(FORBIDDEN)                                                                         \
    X(NOT_FOUND)                                                                         \
    X(METHOD_NOT_ALLOWED)                                                                \
    X(NOT_ACCEPTABLE)                                                                    \
    X(PROXY_AUTHENTICATION_REQUIRED)                                                     \
    X(REQUEST_TIMEOUT)                                                                   \
    X(CONFLICT)                                                                          \
    X(GONE)                                                                              \
    X(LENGTH_REQUIRED)                                                                   \
    X(PRECONDITION_FAILED)                                                               \
    X(PAYLOAD_TOO_LARGE)                                                                 \
    X(URI_TOO_LONG)                                                                      \
    X(UNSUPPORTED_MEDIA_TYPE)                                                            \
    X(RANGE_NOT_SATISFIABLE)                                                             \
    X(EXPECTATION_FAILED)                                                                \
    X(IM_A_TEAPOT)                                                                       \
    X(MISDIRECTED_REQUEST)                                                               \
    X(UNPROCESSABLE_ENTITY)                                                              \
    X(LOCKED)                                                                            \
    X(FAILED_DEPENDENCY)                                                                 \
    X(TOO_EARLY)                                                                         \
    X(UPGRADE_REQUIRED)                                                                  \
    X(PRECONDITION_REQUIRED)                                                             \
    X(TOO_MANY_REQUESTS)                                                                 \
    X(REQUEST_HEADER_FIELDS_TOO_LARGE)                                                   \
    X(UNAVAILABLE_FOR_LEGAL_REASONS)                                                     \
    X(INTERNAL_SERVER_ERROR)                                                             \
    X(NOT_IMPLEMENTED)                                                                   \
    X(BAD_GATEWAY)                                                                       \
    X(SERVICE_UNAVAILABLE)                                                               \
    X(GATEWAY_TIMEOUT)                                                                   \
    X(HTTP_VERSION_NOT_SUPPORTED)                                                        \
    X(VARIANT_ALSO_NEGOTIATES)                                                           \
    X(INSUFFICIENT_STORAGE)                                                              \
    X(LOOP_DETECTED)                                                                     \
    X(NOT_EXTENDED)

enum STATUS_CODE {
    CONTINUE                        = 100,
    SWITCHING_PROTOCOLS             = 101,
    PROCESSING                      = 102,
    EARLY_HINTS                     = 103,
    OK                              = 200,
    CREATED                         = 201,
    ACCEPTED                        = 202,
    NON_AUTHORITATIVE_INFORMATION   = 203,
    NO_CONTENT                      = 204,
    RESET_CONTENT                   = 205,
    PARTIAL_CONTENT                 = 206,
    MULTI_STATUS                    = 207,
    ALREADY_REPORTED                = 208,
    IM_USED                         = 226,
    MULTIPLE_CHOICES                = 300,
    MOVED_PERMANENTLY               = 301,
    FOUND                           = 302,
    SEE_OTHER                       = 303,
    NOT_MODIFIED                    = 304,
    USE_PROXY                       = 305,
    TEMPORARY_REDIRECT              = 307,
    PERMANENT_REDIRECT              = 308,
    BAD_REQUEST                     = 400,
    UNAUTHORIZED                    = 401,
    PAYMENT_REQUIRED                = 402,
    FORBIDDEN                       = 403,
    NOT_FOUND                       = 404,
    METHOD_NOT_ALLOWED              = 405,
    NOT_ACCEPTABLE                  = 406,
    PROXY_AUTHENTICATION_REQUIRED   = 407,
    REQUEST_TIMEOUT                 = 408,
    CONFLICT                        = 409,
    GONE                            = 410,
    LENGTH_REQUIRED                 = 411,
    PRECONDITION_FAILED             = 412,
    PAYLOAD_TOO_LARGE               = 413,
    URI_TOO_LONG                    = 414,
    UNSUPPORTED_MEDIA_TYPE          = 415,
    RANGE_NOT_SATISFIABLE           = 416,
    EXPECTATION_FAILED              = 417,
    IM_A_TEAPOT                     = 418,
    MISDIRECTED_REQUEST             = 421,
    UNPROCESSABLE_ENTITY            = 422,
    LOCKED                          = 423,
    FAILED_DEPENDENCY               = 424,
    TOO_EARLY                       = 425,
    UPGRADE_REQUIRED                = 426,
    PRECONDITION_REQUIRED           = 428,
    TOO_MANY_REQUESTS               = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS   = 451,
    INTERNAL_SERVER_ERROR           = 500,
    NOT_IMPLEMENTED                 = 501,
    BAD_GATEWAY                     = 502,
    SERVICE_UNAVAILABLE             = 503,
    GATEWAY_TIMEOUT                 = 504,
    HTTP_VERSION_NOT_SUPPORTED      = 505,
    VARIANT_ALSO_NEGOTIATES         = 506,
    INSUFFICIENT_STORAGE            = 507,
    LOOP_DETECTED                   = 508,
    NOT_EXTENDED
};

static const std::string sample_response =
    "HTTP/1.1 200 OK\r\n"                        // status line
    "Content-Type: text/html; charset=UTF-8\r\n" // headers
    "Content-Length: 123\r\n"
    "\r\n"
    "<html>\r\n"
    "<head><title>Sample Page</title></head>\r\n"
    "<body><h1>Hello, World!</h1><p>This is a sample webpage.</p></body>\r\n"
    "</html>\r\n";

static const std::string sample_request_cgi =
    "GET /Users/hashim/Desktop/42curses/webserv/sources/CGI/.tests/file.sh?name=hashim&lastname=mohamed HTTP/1.1\r\n" // request-line
    "Host: Linode.com\r\n" // headers fields
    "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.8) "
    "Gecko/20091102 Firefox/3.5.5 \r\n"
    "Accept-Encoding: gzip,deflate\r\n"
    "Accept-Charset: ISO-8859-1,utf-8\r\n"
    "Cache-Control: no-cache\r\n"
    "\r\n" //
    "Lorem ipsum dolor sit amet, officia excepteur ex fugiat reprehenderit\r\n"
    "enim labore culpa sint ad nisi Lorem pariatur mollit ex esse exercitation amet.\r\n"
    "Nisi anim cupidatat excepteur officia. Reprehenderit nostrud nostrud ipsum Lorem\r\n"
    "est aliquip amet voluptate voluptate dolor minim nulla est proident. Nostrud\r\n"
    "officia pariatur ut officia. Sit irure elit esse ea nulla sunt ex occaecat "
    "reprehenderit\r\n"
    "commodo officia dolor Lorem duis laboris cupidatat officia voluptate.\r\n"
    "Culpa proident adipisicing id nulla nisi laboris ex in Lorem sunt duis officia "
    "eiusmod.\r\n"
    "Aliqua reprehenderit commodo ex non excepteur duis sunt velit enim.\r\n"
    "Voluptate laboris sint cupidatat ullamco ut ea consectetur et est culpa et culpa "
    "duis.\r\n"
    "\r\n";

#endif // MESSAGE_HPP
