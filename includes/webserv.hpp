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
// [ ] logging
// [ ] split-up function implementations into their own files
// [ ] different strategies
// [ ] testing (with a testing framework)

/* -------------------------------- INCLUDES -------------------------------- */
#include "debug.hpp"
#include <arpa/inet.h>
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
// clang-format off
typedef vector<pair<string, string> > vsp;
typedef map<string, string> HeaderMap;
// clang-format on
// socket_descriptor type
typedef int fd;
/* --------------------------------- MACROS --------------------------------- */
#define ws_tostr(name) #name
#define ws_itoa(number)                                                                  \
    static_cast<const ostringstream&>((ostringstream() << std::dec << number)).str()
/* ----------------------------- ERROR MESSAGES ----------------------------- */
#define THROW_EXCEPTION_WITH_INFO(msg)                                                   \
    do {                                                                                 \
        ostringstream oss;                                                               \
        oss << __FILE__ << ":" << __LINE__ << ": " << R << "error: " << RE << (msg);     \
        throw runtime_error(oss.str());                                                  \
    } while (0)

static string ERR_NULL("Socket: null ptr error");
static string ERR_CREAT("Socket: creation failed");
static string ERR_BIND("Socket: binding failed");
static string ERR_NBIND("Socket: not bound to a port");
static string ERR_LIST("Socket: listen failed");
static string ERR_NLIST("Socket: not listening");
static string ERR_ACCP("Socket: accept failed");

static string ERR_MEMORY_ALLOC("Memory: allocation failed");

/* --------------------------- UTILITY FUNCTIONS ---------------------------- */
enum FileType {
    REG_FILE = 0,
    DIR,
    NO_EXIST,
    NO_PERM,
    UNEXPECTED
};

/**
 * @brief Determine the type of a given file.
 *
 * This function checks the type of a file specified by its path.
 * It can determine if the file is a regular file, a directory,
 * if it does not exist, if there is no permission to read/write,
 * or if it is an unexpected file type.
 *
 * @param file The path to the file.
 * @return FileType indicating the type of the file or an error.
 */
inline FileType get_file_type(const string& file)
{
    struct stat fileInfo;
    if (stat(file.c_str(), &fileInfo) == -1)
        return NO_EXIST;

    if (access(file.c_str(), R_OK | W_OK) == -1)
        return NO_PERM;

    if (S_ISDIR(fileInfo.st_mode))
        return DIR;
    else if (S_ISREG(fileInfo.st_mode))
        return REG_FILE;

    return UNEXPECTED;
}

#endif // WEBSERV_HPP
