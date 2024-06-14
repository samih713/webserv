#include "Logger.hpp"
#include "Message.hpp"
#include "ServerConfig.hpp"
#include "webserv.hpp"
#include <limits>
#include <set>

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

using std::set;

// file related error messages
#define ERR_FILE_EXTENSION "Config: invalid file extension"
#define ERR_FILE           "Config: invalid file: "
#define ERR_FILE_TYPE      "Config: file is a directory"
#define ERR_FILE_PERM      "Config: invalid file permissions"
#define ERR_OPEN           "Config: cannot open file"
#define ERR_EMPTY          "Config: file is empty"

// brace related error messages
#define ERR_CLOSING_BRACE     "Config: } missing"
#define ERR_OPENING_BRACE     "Config: { missing"
#define ERR_MISSING_SEMICOLON "Config: missing semicolon at "
#define ERR_MISSING_CONTEXT   "Config: missing context"

// global context error messages
#define ERR_TOKENS           "Config: Unexpected tokens found in the global context"
#define ERR_MULTIPLE_DEFAULT "Config: multiple default servers found"

// server context error messages
#define ERR_MISSING_SERVER "Config: missing server context"
#define ERR_SERVER_TOKENS  "Config: Unexpected tokens in the server context"

// location context error messages
#define ERR_URI_MISSING_SLASH   "Config: URI is missing / in the beginning"
#define ERR_URI_DUPLICATE_SLASH "Config: location URI contains multiple /"
#define ERR_LOCATION_TOKENS     "Config: Unexpected tokens in location context"
#define ERR_DUPLICATE_LOCATION  "Config: duplicate location URI"

// listen directive error messages
#define ERR_LISTEN         "Config: invalid listen directive"
#define ERR_MULTIPLE_COLON "Config: multiple colons used"
#define ERR_HOST           "Config: invalid host value"
#define ERR_PORT           "Config: invalid port value"

// server_name directive error messages
#define ERR_SERVER_NAME "Config: invalid server_name directive"

// autoindex directive error messages
#define ERR_AUTOINDEX "Config: invalid autoindex directive"

// root directive error messages
#define ERR_MISSING_ROOT "Config: missing root directive"

// index directive error messages
#define ERR_INDEX "Config: invalid index directive"

// client_max_body_size directive error messages
#define ERR_INVALID_CHAR       "Config: invalid character in client_max_body_size"
#define ERR_MULTIPLE_SUFFIX    "Config: multiple suffixes found"
#define ERR_MISSING_SIZE       "Config: missing size value"
#define ERR_MISSING_SUFFIX     "Config: missing size suffix"
#define ERR_BODY_SIZE_OVERFLOW "Config: body size is out of bounds"

// error_page directive error messages
#define ERR_MISSING_ERROR_CODE "Config: error code missing"
#define ERR_ERROR_PATH         "Config: invalid error page path"

// allow_methods directive error messages
#define ERR_METHOD          "Config: invalid method"
#define ERR_MISSING_METHODS "Config: missing allow_methods"
#define ERR_EMPTY_METHODS   "Config: no allowed methods found"

// redirect directive error messages
#define ERR_REDIRECT_SLASH     "Config: URI is missing / in the beginning"
#define ERR_REDIRECT_DUP_SLASH "Config: location URI contains multiple /"

#define MAX_PORT 65535

vector<ServerConfig> parse_config_file(const string& filePath);

#endif // CONFIG_PARSER_HPP
