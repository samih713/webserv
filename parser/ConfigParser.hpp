#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <map>

namespace ConfigParser {
    union JSON_value {
        int integer;
        double double_value;
        std::map<std::string, JSON_value>* json;
    };

    void parseJSON(std::string file);
    void readFile(const std::string& filepath, std::string& output);
};

#endif // CONFIG_PARSER_HPP