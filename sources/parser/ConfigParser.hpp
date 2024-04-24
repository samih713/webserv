#include "ServerConfig.hpp"
#include "webserv.hpp"
#include <set>
#include <stack>
#include <sys/stat.h>

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

using std::set;
using std::stack;

// file related error messages
static const string ERR_FILE("Config: invalid file");
static const string ERR_STAT("Config: not a regular file");
static const string ERR_OPEN("Config: cannot open file");
static const string ERR_EMPTY("Config: file is empty");

// brace related error messages
static const string ERR_CLOSING_BRACE("Config: } missing");
static const string ERR_OPENING_BRACE("Config: { missing");
static const string ERR_MISSING_SEMICOLON("Config: missing semicolon");
static const string ERR_MISSING_CONTEXT("Config: missing context");

// global context error messages
static const string ERR_TOKENS("Config: Unexpected tokens found in the global context");

// http context error messages
static const string ERR_MISSING_HTTP("Config: missing HTTP context");
static const string ERR_HTTP_TOKENS("Config: Unexpected tokens in the HTTP context");

// server context error messages
static const string ERR_MISSING_SERVER("Config: missing server context");
static const string ERR_SERVER_TOKENS("Config: Unexpected tokens in the server context");

// location context error messages
static const string ERR_LOCATION("Config: invalid location context");
static const string ERR_LOCATION_PATH("Config: location path missing");
static const string ERR_LOCATION_TOKENS("Config: Unexpected tokens in location context");

// listen directive error messages
static const string ERR_LISTEN("Config: invalid listen directive");
static const string ERR_MULTIPLE_COLON("Config: multiple colons used");
static const string ERR_HOST("Config: invalid host value");
static const string ERR_PORT("Config: invalid port value");

// generic directive error messages
static const string ERR_SERVER_NAME("Config: invalid server_name directive");
static const string ERR_ROOT("Config: invalid root directive");
static const string ERR_MISSING_ROOT("Config: missing root directive");
static const string ERR_INDEX("Config: invalid index directive");
static const string ERR_AUTOINDEX("Config: invalid autoindex directive");

// client_max_body_size directive error messages
static const string ERR_INVALID_CHAR("Config: invalid character in client_max_body_size");
static const string ERR_MULTIPLE_SUFFIX("Config: multiple suffixes found");
static const string ERR_MISSING_SIZE("Config: missing size value");
static const string ERR_MISSING_SUFFIX("Config: missing size suffix");
static const string ERR_BODY_SIZE_OVERFLOW("Config: body size is out of bounds");

// error_page directive error messages
static const string ERR_MISSING_ERROR_PATH("Config: error page path missing");
static const string ERR_MISSING_ERROR_CODE("Config: error code missing");
static const string ERR_ERROR_PATH("Config: invalid error page path");


#define MAX_PORT 65535

// TODO:
// [ ] check for duplicate locations
// [ ] check for duplicate indexes
// [ ] check for duplicate error pages
// [ ] empty root can cause problems even if it's valid
// [ ] handle cgi related directives
// [ ] add directive to set http methods allowed

#define NUM_KEYWORDS 10

const string keywords[NUM_KEYWORDS] = { "http", "server", "listen", "server_name",
    "location", "root", "index", "error_page", "client_max_body_size", "autoindex" };

class ConfigParser {
public:
    ConfigParser(const string& filepath);
    ~ConfigParser() {}

    vector<ServerConfig> parse(void);

private:
    vector<string>                 _tokens;
    vector<string>::const_iterator _itr;

    // parsing config contexts
    vector<ServerConfig> _parse_HTTP_context(void);
    ServerConfig         _parse_server_context(void);
    Location             _parse_location_context(void);

    // parsing config directives
    vector<string> _parse_index(const string& root);
    void   _parse_error_page(map<STATUS_CODE, string>& errorPages, const string& root);
    fd     _parse_listen(in_addr_t& serverAddr);
    void   _parse_server_name(string& serverName);
    string _parse_root(void);
    size_t _parse_client_max_body_size(void);
    bool   _parse_autoindex(void);

    bool _is_number(const string& str)
    {
        if (str.find_first_not_of("0123456789") != string::npos)
            return false;
        return true;
    }
    bool _is_keyword(const string& str)
    {
        for (size_t i = 0; i < NUM_KEYWORDS; ++i)
            if (str == keywords[i])
                return true;
        return false;
    }
    void _check_semicolon(void)
    {
        if (*(_itr + 1) != ";")
            THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SEMICOLON);
        ++_itr; // move to semicolon
    }
    void _check_duplicate_directive(set<string>& parsedDirectives, const string& directive)
    {
        if (parsedDirectives.find(directive) != parsedDirectives.end())
            THROW_EXCEPTION_WITH_INFO("Config: duplicate " + directive + " found");

        parsedDirectives.insert(directive);
    }
};

#endif // CONFIG_PARSER_HPP
