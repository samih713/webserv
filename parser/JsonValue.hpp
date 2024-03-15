#ifndef JSON_VALUE_HPP
#define JSON_VALUE_HPP

#include "../includes/webserv.hpp"

class JsonValue {
public:
    enum Type { JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT };

    JsonValue(): _type(JSON_NULL) {}
    JsonValue(const std::string& string): _type(JSON_STRING), _string(string) {}
    JsonValue(int number): _type(JSON_NUMBER), _number(number) {}
    JsonValue(bool boolean): _type(JSON_BOOL), _boolean(boolean) {}
    JsonValue(const std::vector<JsonValue>& array): _type(JSON_ARRAY), _array(array) {}
    JsonValue(const std::map<std::string, JsonValue>& object): _type(JSON_OBJECT), _object(object) {}
    ~JsonValue() {};

    Type getType() const { return _type; };

    bool asBoolean() const { return _boolean; };
    int asNumber() const { return _number; };
    const std::string& asString() const { return _string; };
    const std::vector<JsonValue>& asArray() const { return _array; };
    const std::map<std::string, JsonValue>& asObject() const { return _object; };

    typedef std::map<std::string, JsonValue>::const_iterator objectIterator;
    objectIterator begin() const { return _object.begin(); }
    objectIterator end() const { return _object.end(); }

    typedef std::vector<JsonValue>::const_iterator arrayIterator;
    arrayIterator beginArray() const { return _array.begin(); }
    arrayIterator endArray() const { return _array.end(); }

private:
    Type _type;
    int _number;
    bool _boolean;
    std::string _string;
    std::vector<JsonValue> _array;
    std::map<std::string, JsonValue> _object;
};

#endif // JSON_VALUE_HPP