#ifndef WEBSERV_HPP
#define WEBSERV_HPP


/* --------------------------------- MACROS --------------------------------- */
#define to_str(name) #name

/* -------------------------------- INCLUDES -------------------------------- */
#include <iostream>
#include <string>
#include "debug.hpp"

/* ----------------------------- ERROR MESSAGES ----------------------------- */
static std::string ERR_NULL("Socket: null ptr error");
static std::string ERR_CREAT("Socket: creation failed");
static std::string ERR_BIND("Socket: binding failed");
static std::string ERR_NBIND("Socket: not bound to a port");
static std::string ERR_LIST("Socket: listen failed");
static std::string ERR_NLIST("Socket: not listeing");
static std::string ERR_ACCP("Socket: accept failed");

#endif // WEBSERV_HPP
