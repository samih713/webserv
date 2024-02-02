#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../includes/webserv.hpp"
#include <fstream>
#include <map>
#include <cstring>

class JsonValue {
public:
    enum Type { JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

    JsonValue(): _type(JSON_NULL) {}
    ~JsonValue() {};

    void setNull() { _type = JSON_NULL; };
    void setBoolean(bool boolean) { _type = JSON_BOOL; _boolean = boolean; };
    void setNumber(int number) { _type = JSON_NUMBER; _number = number; };
    void setString(const std::string& string) { _type = JSON_STRING; _string = string; };
    void setArray(const std::vector<JsonValue>& array) { _type = JSON_ARRAY; _array = array; };
    void setObject(const std::map<std::string, JsonValue>& object) { _type = JSON_OBJECT; _object = object; };

    Type getType() const { return _type; };

    bool asBoolean() const { return _boolean; };
    int asNumber() const { return _number; };
    const std::string& asString() const { return _string; };
    const std::vector<JsonValue>& asArray() const { return _array; };
    const std::map<std::string, JsonValue>& asObject() const { return _object; };

private:
    Type _type;
    union {
        int _number;
        bool _boolean;
    };
    std::string _string;
    std::vector<JsonValue> _array;
    std::map<std::string, JsonValue> _object;
};

typedef std::string::iterator stringIterator;

class JsonParser {
public:
    JsonParser(const std::string filepath);
    ~JsonParser() {};

    JsonValue parseJSON(void);
    void printJSON(JsonValue json);

private:
    std::string _content;
    stringIterator _itr;
    stringIterator _itr2;

    JsonValue parseValue(void);
    JsonValue parseNull(void);
    JsonValue parseString(void);
    JsonValue parseNumber(void);
    JsonValue parseBoolean(void);
    JsonValue parseArray(void);
    JsonValue parseObject(void);
};

#endif // CONFIG_PARSER_HPP