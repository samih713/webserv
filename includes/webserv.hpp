#ifndef WEBSERV_HPP
#define WEBSERV_HPP

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
#include <string>
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
