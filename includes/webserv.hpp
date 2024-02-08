#ifndef WEBSERV_HPP
#define WEBSERV_HPP

/* --------------------------------- MACROS --------------------------------- */
#define to_str(name) #name

/* -------------------------------- INCLUDES -------------------------------- */
#include "debug.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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
