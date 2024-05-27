#include "CachedPages.hpp"
#include "Message.hpp"
#include "Response.hpp"
#include "webserv.hpp"

#ifndef ERROR_PAGES_HPP
#define ERROR_PAGES_HPP

Page get_error_page(STATUS_CODE status, CachedPages& cpages, ServerConfig& config);

#endif // ERROR_PAGES_HPP
