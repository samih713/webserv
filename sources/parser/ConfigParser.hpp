#include "../../includes/webserv.hpp"
#include "../server/ServerConfig.hpp"
#include <sys/stat.h>
#include <stack>

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

using std::stack;
using std::runtime_error;

static string ERR_INVALID_FILE("Parser: invalid file");
static string ERR_STAT("Parser: not a regular file");
static string ERR_OPEN("Parser: cannot open file");
static string ERR_EMPTY("Parser: file is empty");

static string ERR_CLOSING_BRACE("Parser: } missing");
static string ERR_OPENINING_BRACE("Parser: { missing");
static string ERR_MISSING_SEMICOLON("Parser: missing semicolon");
static string ERR_MISSING_CONTEXT("Parser: missing context");

static string ERR_MISSING_HTTP("Parser: missing HTTP context");
static string ERR_UNEXPECTED_TOKENS_IN("Parser: Unexpected tokens found inside the HTTP context");
static string ERR_UNEXPECTED_TOKENS_OUT("Parser: Unexpected tokens found outside the HTTP context");

static string ERR_MISSING_SERVER("Parser: missing server context");
static string ERR_UNEXPECTED_TOKENS_IN_SERVER("Parser: Unexpected tokens found inside the server context");
static string ERR_INVALID_LISTEN("Parser: invalid listen directive");
static string ERR_INVALID_SERVER_NAME("Parser: invalid server_name directive");
static string ERR_INVALID_ROOT("Parser: invalid root directive");
static string ERR_MISSING_ROOT("Parser: missing root directive");
static string ERR_INVALID_INDEX("Parser: invalid index directive");
static string ERR_INVALID_AUTOINDEX("Parser: invalid autoindex directive");
static string ERR_INVALID_BODY_SIZE("Parser: invalid client_max_body_size directive");

static string ERR_ERROR_PATH("Parser: error page path missing");
static string ERR_ERROR_CODE("Parser: error page code missing");
static string ERR_INVALID_ERROR_PATH("Parser: invalid error page path");

static string ERR_INVALID_LOCATION("Parser: invalid location context");
static string ERR_LOCATION_PATH("Parser: location path missing");
static string ERR_UNEXPECTED_TOKENS_IN_LOCATION("Parser: Unexpected tokens found inside the location context");

#define MAX_PORT 65535

// TODO:
// [ ] check for duplicate server names
// [ ] check for duplicate listen ports
// [ ] check for duplicate locations
// [ ] check for duplicate indexes
// [ ] check for duplicate error pages
// [ ] check for duplicate autoindex (depends on context)
// [ ] empty root can cause problems even if it's valid
// [ ] check for duplicate client_max_body_size
// [ ] handle cgi related directives
// [ ] add directive to set http methods allowed

#define NUM_KEYWORDS 10

const string keywords[NUM_KEYWORDS] = {
    "http",
    "server",
    "listen",
    "server_name",
    "location",
    "root",
    "index",
    "error_page",
    "client_max_body_size",
    "autoindex"
};

class ConfigParser {
public:
    ConfigParser(string const& filepath);
    ~ConfigParser() {};

    vector<ServerConfig> parse(void);

private:
    string _content;
    vector<string> _tokens;
    vector<string>::const_iterator _itr;

    vector<ServerConfig> _parse_HTTP_context(void);
    ServerConfig         _parse_server_context(void);
    Location             _parse_location_context(void);
    vector<string>       _parse_index(string const& root);
    void                 _parse_error_page(map<STATUS_CODE, string>& errorPages, string const& root);
    fd                   _parse_listen(void);
    void                 _parse_server_name(vector<string>& serverName);
    string               _parse_root(void);
    string               _parse_client_max_body_size(void);
    bool                 _parse_autoindex(void);

    bool _is_string_number(const string& str) {
        if (str.find_first_not_of("0123456789") != string::npos)
            return false;
        return true;
    }
    bool _is_keyword(const string& str) {
        for (size_t i = 0; i < NUM_KEYWORDS; ++i) {
            if (str == keywords[i])
                return true;
        }
        return false;
    }
    void _check_semicolon(void) {
        if (*(_itr + 1) != ";")
            throw runtime_error(ERR_MISSING_SEMICOLON);
        ++_itr; // move to semicolon
    }
};

#endif // CONFIG_PARSER_HPP