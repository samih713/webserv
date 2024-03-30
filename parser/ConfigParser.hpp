#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include "../server/Config.hpp"
#include <sys/stat.h>
#include <stack>

using std::stack;
using std::runtime_error;

static string ERR_INVALID_FILE("Parser: invalid file");
static string ERR_STAT("Parser: not a regular file");
static string ERR_OPEN("Parser: cannot open file");
static string ERR_EMPTY("Parser: file is empty");
static string ERR_CLOSING_BRACE("Parser: } missing");
static string ERR_OPENINING_BRACE("Parser: { missing");
static string ERR_MISSING_SEMICOLON("Parser: semicolon missing");
static string ERR_MISSING_SERVER("Parser: server block missing");

class ConfigParser {
public:
    ConfigParser(std::string const& filepath);
    ~ConfigParser() {};

    void parse(void);

private:
    std::string _content;
    std::vector<std::string> _tokens;
    Config _config;

    void _validate(void);
};

#endif // CONFIG_PARSER_HPP