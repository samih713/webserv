#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* ---------------------------------- TODO ---------------------------------- */

// SERVER
// [ ] resource handling for get-requests, is broken
// [ ] better option handling
// [ ] add vector<page> to CachedPages
// [ ] ensure that the backlog isn't greater than 1024 due to select limitation

// REQUEST
// [ ] better error messages, stream throws no way to know where is error
// [ ] headers to parse multiple line field-values, and multi-line
// [ ] need to form proper error response in case of parsing failure
// [ ] handle chunked encoding
// [ ] parsing needs to be reviewed for white space parsing

// CGI
// [ ] fix forking here for sleep
// [ ] logging
// [ ] split-up function implementations into their own files
// [ ] different strategies
// [ ] testing (with a testing framework)

/* -------------------------------- WARNINGS -------------------------------- */

// PARSER
// [ ] maybe there's stuff in the global context that need to be parsed

/* --------------------------------- MACROS --------------------------------- */
#define ws_tostr(name) #name
#define ws_itoa(number)                                                                  \
    static_cast<const std::ostringstream&>((std::ostringstream() << std::dec << number)) \
        .str()

/* -------------------------------- INCLUDES -------------------------------- */
#include "./debug.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
/* --------------------------------- USING ---------------------------------- */
using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::map;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;
/* -------------------------------- TYPEDEFS -------------------------------- */
// clang-format off
typedef vector<pair<string, string> > vsp;
// clang-format on
// socket_descriptor type
typedef int fd;
/* ----------------------------- ERROR MESSAGES ----------------------------- */
#define THROW_EXCEPTION_WITH_INFO(msg)                                                   \
    do {                                                                                 \
        std::ostringstream oss;                                                          \
        oss << __FILE__ << ":" << __LINE__ << ": " << R << "error: " << RE << (msg);     \
        throw std::runtime_error(oss.str());                                             \
    } while (0)

static std::string ERR_NULL("Socket: null ptr error");
static std::string ERR_CREAT("Socket: creation failed");
static std::string ERR_BIND("Socket: binding failed");
static std::string ERR_NBIND("Socket: not bound to a port");
static std::string ERR_LIST("Socket: listen failed");
static std::string ERR_NLIST("Socket: not listening");
static std::string ERR_ACCP("Socket: accept failed");

static std::string ERR_MEMORY_ALLOC("Memory: allocation failed");

#endif // WEBSERV_HPP
