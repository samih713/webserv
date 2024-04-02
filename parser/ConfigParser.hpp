#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include "../server/Config.hpp"
#include <sys/stat.h>
#include <stack>
#include "../server/Config.hpp"

using std::stack;
using std::runtime_error;

static string ERR_INVALID_FILE("Parser: invalid file");
static string ERR_STAT("Parser: not a regular file");
static string ERR_OPEN("Parser: cannot open file");
static string ERR_EMPTY("Parser: file is empty");
static string ERR_CLOSING_BRACE("Parser: } missing");
static string ERR_OPENINING_BRACE("Parser: { missing");
static string ERR_MISSING_SEMICOLON("Parser: semicolon missing");
static string ERR_MISSING_CONTEXT("Parser: missing context");
static string ERR_MISSING_HTTP("Parser: missing HTTP context");
static string ERR_MULTIPLE_HTTP("Parser: multiple HTTP contexts found");
static string ERR_MISSING_SERVER("Parser: missing server context");

// TODO:
// * Add more error messages
// * Parse context blocks and fill into _config
// * For semi-colon missing, check if it's the last line of the block by checking for the required values
// * maybe remove the ConfigParser class and put the parse function in the Config class

class ConfigParser {
public:
    ConfigParser(string const& filepath);
    ~ConfigParser() {};

    Config parse(void);

private:
    string _content;
    vector<string> _tokens;
    Config _config;
};

#endif // CONFIG_PARSER_HPP