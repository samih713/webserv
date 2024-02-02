#include "ConfigParser.hpp"

JsonParser::JsonParser(const std::string filepath) {
    std::cout << "Reading config file: " << filepath << std::endl;

    std::ifstream inputFileStream(filepath.c_str());
    if (inputFileStream.fail())
        throw std::runtime_error(ERR_OPEN);

    std::string line;
    while (std::getline(inputFileStream, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.empty())
            continue;
        _content.append(line);
    }
    inputFileStream.close();
    _itr = _content.begin();
}

JsonValue JsonParser::parseJSON(void) {
    _itr2 = _content.begin();
    std::cout << "Parsing JSON" << std::endl;
    return parseValue();
}

JsonValue JsonParser::parseValue(void) {
    if (_itr == _content.end())
        throw std::runtime_error(ERR_JSON_PARSE);

    if (*_itr == '\"')
        return parseString();
    else if (isdigit(*_itr))
        return parseNumber();
    else if (*_itr == 'f' || *_itr == 't')
        return parseBoolean();
    else if (std::string(_itr, _itr + 4) == "null")
        return parseNull();
    else if (*_itr == '[')
        return parseArray();
    else if (*_itr == '{')
        return parseObject();
    else
        throw std::runtime_error(ERR_JSON_PARSE);
}

JsonValue JsonParser::parseNull(void) {
    // _itr moved from 'n' to after 'l'.
    // ',' or '}' should be right after
    _itr += 4;
    if (*_itr != ',' && *_itr != '}')
        throw std::runtime_error(ERR_JSON_PARSE);
    JsonValue result;
    result.setNull();
    return result;
}

JsonValue JsonParser::parseString(void) {
    // _itr moved from '\"' to after '\"'
    // ',' or '}' or ':' or ']' should be right after
    _itr++; // move from '\"'
    stringIterator tempItr = _itr;
    while (_itr != _content.end() && *_itr != '\"')
        _itr++;
    if (*_itr != '\"')
        throw std::runtime_error(ERR_JSON_QUOTE);
    std::string value(tempItr, _itr);
    _itr++; // move from '\"'
    if (*_itr != ',' && *_itr != '}' && *_itr != ']' && *_itr != ':')
        throw std::runtime_error(ERR_JSON_PARSE);
    JsonValue result;
    result.setString(value);
    return result;
}

JsonValue JsonParser::parseNumber(void) {
    // _itr moved from digit to after digit
    // ',' or '}' or ']' should be right after
    stringIterator tempItr = _itr;
    while (_itr != _content.end() && isdigit(*_itr)) {
        if (*_itr == '.')
            throw std::runtime_error(ERR_JSON_TYPE);
        _itr++;
    }
    int number = std::atoi(std::string(tempItr, _itr).c_str());
    if (_itr == _content.end() || (*_itr != ',' && *_itr != '}' && *_itr != ']'))
        throw std::runtime_error(ERR_JSON_PARSE);
    JsonValue result;
    result.setNumber(number);
    return result;
}

JsonValue JsonParser::parseBoolean(void) {
    // _itr moved from 't' or 'f' to after 'e'
    // ',' or '}' should be right after
    bool boolean;
    if (std::string(_itr, _itr + 4) == "true")
        boolean = true;
    else if (std::string(_itr, _itr + 5) == "false")
        boolean = false;
    else
        throw std::runtime_error(ERR_JSON_PARSE);
    _itr += (boolean ? 4 : 5);
    if (*_itr != ',' && *_itr != '}')
        throw std::runtime_error(ERR_JSON_PARSE);
    JsonValue result;
    result.setBoolean(boolean);
    return result;
}

JsonValue JsonParser::parseArray(void) {
    // _itr moved from '[' to after ']'
    // ',' or '}' should be right after
    _itr++; // move from '['
    if (*_itr != '\"' && !isdigit(*_itr) && *_itr != 'f' && *_itr != 't' && *_itr != 'n' && *_itr != '[' && *_itr != '{')
        throw std::runtime_error(ERR_JSON_PARSE);
    std::vector<JsonValue> array;
    while (_itr != _content.end() && *_itr != ']') {
        array.push_back(parseValue());
        if (*_itr != ',' && *_itr != ']')
            throw std::runtime_error(ERR_JSON_PARSE);
        if (*_itr == ',')
            _itr++;
    }
    if (*_itr != ']' && (*(++_itr) != ',' || *_itr != '}'))
        throw std::runtime_error(ERR_JSON_PARSE);
    _itr++;
    JsonValue result;
    result.setArray(array);
    return result;
}

JsonValue JsonParser::parseObject(void) {
    // _itr moved from '{' to after '}'
    // ',' or '}' or EOF should be right after
    _itr++; // move from '{'
    std::map<std::string, JsonValue> value;
    while (_itr != _content.end() && *_itr != '}') {
        if (*_itr != '\"')
            throw std::runtime_error(ERR_JSON_PARSE);
        std::string key = parseString().asString();
        _itr++; // move from ':'
        value[key] = parseValue();
        if (*_itr != ',' && *_itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
        if (*_itr == ',')
            _itr++;
    }
    if (*(++_itr) != ',' && *_itr != ']' && _itr != _content.end()) {
        std::cout << *_itr << std::endl;
        throw std::runtime_error(ERR_JSON_PARSE);
    }
    JsonValue result;
    result.setObject(value);
    return result;
}
