#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <map>
#include <cstring>
#include <sys/stat.h>
#include "JsonParser.hpp"

class ConfigParser {
public:
    ConfigParser(const std::string filepath);
    ~ConfigParser() {};

    JsonValue parseConfig(void);

private:
    std::string _content;
};

#endif // CONFIG_PARSER_HPP