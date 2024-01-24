#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>

class ConfigParser {
public:
    ~ConfigParser();

    static void parseJSON(std::string file);

private:
    ConfigParser();
};

#endif // CONFIG_PARSER_HPP