#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <map>

namespace ConfigParser {
    union JsonValue {
        int number;
        std::string* string; // pointer to string to avoid copying
        std::map<std::string, JsonValue>* json;
    };

    typedef std::pair<std::string, JsonValue> KeyValuePair;
    typedef std::string::iterator stringIterator;

    void parseJSON(const std::string file);
    void readFile(const std::string& filepath, std::string& content);
    KeyValuePair parseKeyValuePair(const std::string& content, stringIterator& itr);
};

#endif // CONFIG_PARSER_HPP