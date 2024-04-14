#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* ---------------------------------- TODO ---------------------------------- */

// SERVER
// [ ] hanlde partial sends and recieves (Server::handle_connection)
// [ ] better option handling
// [ ] add vector<page> to CachedPages
// [ ] finish up the resource handling for get-requests
// [ ] strict space parsing (only 1 space)
// [ ] Address sanitizer error when testing with ./test_server
// [ ] ensure that the backlog isn't greater than 1024
// [ ] find a better way to include server name
// [x] solve linkage issue (with data)
// [x] Server constructor needs to handle socket creation failure
// [x] handle keep alive, currently not closing the scoket
// [x] 404 page loading needs to happen once in the server/serve the css page aswell
// [x] Implement response
// [x] set the stream to throw exception on fail
// [x] sometimes segfaults when parsing, keep repeating to reproduce

// REQUEST
// [ ] why getting method not implemented in debug mode (run server to replicate)
// [ ] headers to parse multiple line field-values, and multi-line
// [ ] need to form proper error response in case of parsing failure
// [ ] handle chunked encoding
// [ ] parsing needs to be reviewed for white space parsing


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
    static_cast<const std::ostringstream &>(                                             \
        (std::ostringstream() << std::dec << number))                                    \
        .str()

/* -------------------------------- INCLUDES -------------------------------- */
#include "./debug.hpp"
#include <algorithm>
#include <cerrno>
#include <cstddef>
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
static std::string ERR_NULL("Socket: null ptr error");
static std::string ERR_CREAT("Socket: creation failed");
static std::string ERR_BIND("Socket: binding failed");
static std::string ERR_NBIND("Socket: not bound to a port");
static std::string ERR_LIST("Socket: listen failed");
static std::string ERR_NLIST("Socket: not listening");
static std::string ERR_ACCP("Socket: accept failed");

static std::string ERR_MEMORY_ALLOC("Memory: allocation failed");

#endif // WEBSERV_HPP
