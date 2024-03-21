#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <sys/stat.h>
#include <stack>

class ConfigParser {
public:
    ConfigParser(std::string const& filepath);
    ~ConfigParser() {};

    void parse(void);

private:
    std::string _content;
    std::vector<std::string> _tokens;

    ConfigParser();
    void _validate(void);
};

#endif // CONFIG_PARSER_HPP