#include "Message.hpp"
#include "ServerConfig.hpp"
#include "webserv.hpp"
#include <limits>
#include <set>

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

using std::set;

// file related error messages
static const string ERR_FILE_EXTENSION("Config: invalid file extension");
static const string ERR_FILE("Config: invalid file: ");
static const string ERR_FILE_TYPE("Config: file is a directory");
static const string ERR_FILE_PERM("Config: invalid file permissions");
static const string ERR_OPEN("Config: cannot open file");
static const string ERR_EMPTY("Config: file is empty");

// brace related error messages
static const string ERR_CLOSING_BRACE("Config: } missing");
static const string ERR_OPENING_BRACE("Config: { missing");
static const string ERR_MISSING_SEMICOLON("Config: missing semicolon at ");
static const string ERR_MISSING_CONTEXT("Config: missing context");

// global context error messages
static const string ERR_TOKENS("Config: Unexpected tokens found in the global context");
static const string ERR_MULTIPLE_DEFAULT("Config: multiple default servers found");

// server context error messages
static const string ERR_MISSING_SERVER("Config: missing server context");
static const string ERR_SERVER_TOKENS("Config: Unexpected tokens in the server context");

// location context error messages
static const string ERR_URI_MISSING_SLASH("Config: URI is missing / in the beginning");
static const string ERR_URI_DUPLICATE_SLASH("Config: location URI contains multiple /");
static const string ERR_LOCATION_TOKENS("Config: Unexpected tokens in location context");
static const string ERR_DUPLICATE_LOCATION("Config: duplicate location URI");
static const string ERR_MISSING_LOCATION("Config: location context missing");
static const string ERR_LOCATION_INDEX("Config: cannot use index for this URI");

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

// redirect directive error messages
static const string ERR_REDIRECT("Config: invalid redirect directive");
static const string ERR_REDIRECT_SLASH("Config: URI is missing / in the beginning");
static const string ERR_REDIRECT_DUP_SLASH("Config: location URI contains multiple /");

#define MAX_PORT 65535


#define NUM_KEYWORDS 15

const string keywords[NUM_KEYWORDS] = { "server", "listen", "server_name", "location",
    "root", "index", "error_page", "client_max_body_size", "autoindex", "allow_methods" };

class ConfigParser {
public:
    static ConfigParser& get_instance(const string& filepath)
    {
        static ConfigParser singleton(filepath);
        return singleton;
    }
    ~ConfigParser() {}

    vector<ServerConfig> parse(void);

private:
    vector<string>                 _tokens;
    vector<string>::const_iterator _itr;

    ConfigParser(const string& filepath);

    // parsing config contexts
    ServerConfig parse_server_context(void);
    void         parse_location_context(ServerConfig& server);

    // parsing config directives
    vector<string> parse_allow_methods(void);
    bool           parse_autoindex(void);
    size_t         parse_client_max_body_size(void);
    void           parse_error_page(StatusCodeMap& errorPages, const string& root);
    fd             parse_listen(in_addr_t& host, bool& defaultServer);
    void           parse_http_redirection(RedirectionMap& redirMap);
    void           parse_index(string& indexFile, const string& root);
    void           parse_root(string& root);
    void           parse_server_name(string& serverName);

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
    void check_duplicate_element(set<string>& container, const string& element,
        const string& context)
    {
        if (container.find(element) != container.end())
            THROW_EXCEPTION_WITH_INFO(
                "Config: duplicate " + element + " found in " + context + " context");

        container.insert(element);
    }
};

#endif // CONFIG_PARSER_HPP
