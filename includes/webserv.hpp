#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* ---------------------------------- TODO ---------------------------------- */

// SOCKETS
// [ ] add shutdown functionality (block recv, block send, close())

// SERVER
// [ ] 404 page loading needs to happen once in the server/serve the css page aswell
// [ ] handle the connection
// [ ] sessions manager/connection manager // should disconnect ...
// [ ] finish up the resource handling for get-requests
// [ ] strict space parsing (only 1 space)
// [ ] solve linkage issue (with data)
// [ ] (Server.cpp) compare bytesReceived with size from headers
// [x] Implement response
// [x] set the stream to throw exception on fail
// [x] sometimes segfaults when parsing, keep repeating to reproduce

// [ ] logging
// [ ] split-up function implementations into their own files
// [ ] different strategies
// [ ] testing (with a testing framework)

/* --------------------------------- MACROS --------------------------------- */
#define ws_tostr(name) #name
#define ws_itoa(number)                                                                  \
    static_cast<const std::ostringstream &>((std::ostringstream() << std::dec << number)).str()

/* -------------------------------- INCLUDES -------------------------------- */
#include "debug.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
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

static std::string ERR_OPEN("Parser: cannot open file");
static std::string ERR_JSON_PARSE("Parser: invalid JSON syntax");
static std::string ERR_JSON_TYPE("Parser: invalid JSON type");
static std::string ERR_JSON_QUOTE("Parser: unclosed quote");

static std::string ERR_MEMORY_ALLOC("Memory: allocation failed");

#endif // WEBSERV_HPP
