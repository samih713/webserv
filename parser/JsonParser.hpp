#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include "../includes/webserv.hpp"
#include "JsonValue.hpp"

class JsonParser {
public:
    JsonParser(const std::string& content): _content(content) { _itr = _content.begin(); };
    ~JsonParser() {};

    JsonValue parseJSON(void);

private:
    std::string _content;
    std::string::iterator _itr;

    JsonValue parseValue(void);
    JsonValue parseNull(void);
    JsonValue parseString(void);
    JsonValue parseNumber(void);
    JsonValue parseBoolean(void);
    JsonValue parseArray(void);
    JsonValue parseObject(void);
    bool isEndOrDelimiter(void) {
        return *_itr == ',' || *_itr == '}' || *_itr == ']' || _itr == _content.end();
    }
};

#endif