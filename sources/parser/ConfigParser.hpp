#include "ServerConfig.hpp"
#include "webserv.hpp"
#include <limits>
#include <set>
#include <stack>
#include <sys/stat.h>

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

using std::set;
using std::stack;

#define DIR  1
#define FILE 2

// file related error messages
static const string ERR_FILE_EXTENSION("Config: invalid file extension");
static const string ERR_FILE("Config: invalid file ");
static const string ERR_OPEN("Config: cannot open file");
static const string ERR_EMPTY("Config: file is empty");

// brace related error messages
static const string ERR_CLOSING_BRACE("Config: } missing");
static const string ERR_OPENING_BRACE("Config: { missing");
static const string ERR_MISSING_SEMICOLON("Config: missing semicolon after ");
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
static const string ERR_LOCATION("Config: invalid character in location uri");
static const string ERR_LOCATION_PATH("Config: location path missing");
static const string ERR_LOCATION_TOKENS("Config: Unexpected tokens in location context");
static const string ERR_DUPLICATE_LOCATION("Config: duplicate location");
static const string ERR_MISSING_LOCATION("Config: location context missing");

// listen directive error messages
static const string ERR_LISTEN("Config: invalid listen directive");
static const string ERR_MULTIPLE_COLON("Config: multiple colons used");
static const string ERR_HOST("Config: invalid host value");
static const string ERR_PORT("Config: invalid port value");

// server_name directive error messages
static const string ERR_SERVER_NAME("Config: invalid server_name directive");

// autoindex directive error messages
static const string ERR_AUTOINDEX("Config: invalid autoindex directive");
static const string ERR_AUTOINDEX_CGI("Config: autoindex is not allowed for CGI");

// root directive error messages
static const string ERR_ROOT("Config: invalid root");
static const string ERR_MISSING_ROOT("Config: missing root directive");

// index directive error messages
static const string ERR_INDEX("Config: invalid index directive");
static const string ERR_INVALID_INDEX("Config: invalid index file");

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

// allow_methods directive error messages
static const string ERR_METHOD("Config: invalid method");
static const string ERR_MISSING_METHODS("Config: missing allow_methods");
static const string ERR_EMPTY_METHODS("Config: no allowed methods found");

#define MAX_PORT 65535

// TODO:
// [ ] check for duplicate error pages
// [ ] handle cgi related directives

#define NUM_KEYWORDS 15

const string keywords[NUM_KEYWORDS] = { "http", "server", "listen", "server_name",
    "location", "root", "index", "error_page", "client_max_body_size", "autoindex",
    "allow_methods" };

class ConfigParser {
public:
    ConfigParser(const string& filepath);
    ~ConfigParser() {}

    vector<ServerConfig> parse(void);

private:
    vector<string>                 _tokens;
    vector<string>::const_iterator _itr;

    // parsing config contexts
    vector<ServerConfig> parse_HTTP_context(void);
    ServerConfig         parse_server_context(void);
    Location             parse_location_context(ServerConfig& server);

    // parsing config directives
    void   parse_index(string& indexFile, const string& root);
    void   parse_error_page(map<STATUS_CODE, string>& errorPages, const string& root);
    fd     parse_listen(in_addr_t& host);
    void   parse_server_name(string& serverName);
    void   parse_root(string& root);
    size_t parse_client_max_body_size(void);
    bool   parse_autoindex(void);
    void   parse_allow_methods(vector<string>& methods);

    int get_file_type(const string& file)
    {
        struct stat fileInfo;
        if (stat(file.c_str(), &fileInfo) == -1)
            THROW_EXCEPTION_WITH_INFO(file + ": " + strerror(errno));

        if (access(file.c_str(), R_OK | W_OK) == -1)
            THROW_EXCEPTION_WITH_INFO(file + ": " + strerror(errno));

        if (S_ISDIR(fileInfo.st_mode))
            return DIR;
        else if (S_ISREG(fileInfo.st_mode))
            return FILE;
        return -1;
    }
    bool is_number(const string& str)
    {
        if (str.find_first_not_of("0123456789") != string::npos)
            return false;
        return true;
    }
    bool is_keyword(const string& str)
    {
        for (size_t i = 0; i < NUM_KEYWORDS; ++i)
            if (str == keywords[i])
                return true;
        return false;
    }
    void check_semicolon(void)
    {
        if (*(_itr + 1) != ";")
            THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SEMICOLON + *_itr);
        ++_itr; // move to semicolon
    }
    void check_duplicate_directive(set<string>& parsedDirectives, const string& directive)
    {
        if (parsedDirectives.find(directive) != parsedDirectives.end())
            THROW_EXCEPTION_WITH_INFO("Config: duplicate " + directive + " found");

        parsedDirectives.insert(directive);
    }
};

#endif // CONFIG_PARSER_HPP
