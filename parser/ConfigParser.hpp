#include "../includes/webserv.hpp"
#include "../server/Config.hpp"
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
static string ERR_INVALID_INDEX("Parser: invalid index directive");
static string ERR_ERROR_PATH("Parser: error page path missing");

static string ERR_INVALID_LOCATION("Parser: invalid location context");
static string ERR_LOCATION_PATH("Parser: location path missing");
static string ERR_UNEXPECTED_TOKENS_IN_LOCATION("Parser: Unexpected tokens found inside the location context");

#define MAX_PORT 65535

// TODO:
// * Parse context blocks and fill into _config
// * For semi-colon missing, check if it's the last line of the block by checking for the required values
// * maybe remove the ConfigParser class and put the parse function in the Config class

const string keywords[8] = {
    "server",
    "listen",
    "server_name",
    "location",
    "root",
    "index",
    "error_page",
    "autoindex"
};

class ConfigParser {
public:
    ConfigParser(string const& filepath);
    ~ConfigParser() {};

    Config parse(void);

private:
    string _content;
    vector<string> _tokens;
    Config _config;
    vector<string>::const_iterator _itr;

    void _validateBraces(void);
    void _parseHTTPContext(void);
    void _parseServerContext(void);
    void _parseLocationContext(void);
    void _parseIndexDirective(void);

    bool _isStringNumber(const string& str) {
        for (string::const_iterator itr = str.begin(); itr != str.end(); ++itr) {
            if (!std::isdigit(*itr))
                return false;
        }
        return true;
    }
    bool _isKeyword(const string& str) {
        for (size_t i = 0; i < 8; ++i) {
            if (str == keywords[i])
                return true;
        }
        return false;
    }
    void _checkSemicolon(void) {
        if (*(_itr + 1) != ";")
            throw runtime_error(ERR_MISSING_SEMICOLON);
        ++_itr; // move to semicolon
    }
};

#endif // CONFIG_PARSER_HPP