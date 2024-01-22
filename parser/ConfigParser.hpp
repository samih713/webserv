#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <string>

class ConfigParser {
public:
    ConfigParser(std::string file);
    ~ConfigParser();

private:
    ConfigParser();

};

#endif