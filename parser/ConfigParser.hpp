#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/error.hpp"
#include "../includes/webserv.hpp"
#include <fstream>

class ConfigParser {
public:
    ~ConfigParser();

    static void parseConfigFile(std::string file);

private:
    ConfigParser();
};

#endif // CONFIG_PARSER_HPP