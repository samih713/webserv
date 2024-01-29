#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <map>

namespace ConfigParser {
    union JsonValue {
        int integer;
        double double_value;
        std::map<std::string, JsonValue>* json;
    };

    void parseJSON(const std::string file);
    void readFile(const std::string& filepath, std::string& content);
};

#endif // CONFIG_PARSER_HPP