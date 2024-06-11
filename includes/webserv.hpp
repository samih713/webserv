#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* ---------------------------------- TODO ---------------------------------- */

// SERVER
// [ ] handle chunked encoding for Responses
// [ ] resource handling for get-requests, is broken
// [ ] better option handling
// [ ] add vector<page> to CachedPages
// [ ] ensure that the backlog isn't greater than 1024 due to select limitation
// [ ] client sends an infinite loop test this case
// [x] handle chunked encoding for Requests

// REQUEST
// https://datatracker.ietf.org/doc/html/rfc7230#section-3.3.3
// [ ] sets status for 400, 501 ... etc
// [ ] replace all other special characters
// [ ] better error messages, stream throws no way to know where is error
// [ ] optimizing in case of similar request, no need to parse again
// [x] handle transfer size
// [x] headers to parse multiple line field-values, and multi-line

// CGI
// [ ] fix forking here for sleep
// [ ] split-up function implementations into their own files
// [ ] different strategies
// [ ] testing (with a testing framework)

/* -------------------------------- INCLUDES -------------------------------- */
#include "debug.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <signal.h>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#if defined(__MAC__)
#include <sys/event.h>
#endif

/* --------------------------------- USING ---------------------------------- */
using std::cerr;
using std::cout;
using std::endl;
using std::fstream;
using std::ifstream;
using std::istream;
using std::istringstream;
using std::make_pair;
using std::map;
using std::ofstream;
using std::ostream;
using std::ostringstream;
using std::pair;
using std::runtime_error;
using std::stack;
using std::string;
using std::stringstream;
using std::vector;

/* -------------------------------- TYPEDEFS -------------------------------- */
typedef map<string, string> HeaderMap;
typedef int                 fd; // socket_descriptor type

/* --------------------------------- MACROS --------------------------------- */
#define ws_itoa(number)                                                                  \
    static_cast<const ostringstream&>((ostringstream() << std::dec << number)).str()

/* ----------------------------- ERROR MESSAGES ----------------------------- */
#define THROW_EXCEPTION_WITH_INFO(msg)                                                   \
    do {                                                                                 \
        ostringstream oss;                                                               \
        oss << __FILE__ << ":" << __LINE__ << ": " << (msg);                             \
        throw runtime_error(oss.str());                                                  \
    } while (0)

static string ERR_CREAT("Socket: creation failed");
static string ERR_BIND("Socket: binding failed");
static string ERR_NBIND("Socket: not bound to a port");
static string ERR_LIST("Socket: listen failed");
static string ERR_NLIST("Socket: not listening");
static string ERR_ACCP("Socket: accept failed");

/* ----------------------------- GLOBAL VARIABLE ---------------------------- */
static volatile sig_atomic_t g_stopServer = 0;

#endif // WEBSERV_HPP
