#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <sys/stat.h>
#include <stack>

static std::string ERR_INVALID_FILE("Parser: invalid file");
static std::string ERR_STAT("Parser: not a regular file");
static std::string ERR_OPEN("Parser: cannot open file");
static std::string ERR_EMPTY("Parser: file is empty");
static std::string ERR_CLOSING_BRACE("Parser: } missing");
static std::string ERR_OPENINING_BRACE("Parser: { missing");
static std::string ERR_MISSING_SEMICOLON("Parser: semicolon missing");

class ConfigParser {
public:
    ConfigParser(std::string const& filepath);
    ~ConfigParser() {};

    void parse(void);

private:
    std::string _content;
    std::vector<std::string> _tokens;

    void _validate(void);
};

#endif // CONFIG_PARSER_HPP