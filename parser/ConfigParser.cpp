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

/**
 * @brief Parse the JSON string
 * 
 * @return JsonValue The parsed JSON value
*/
JsonValue JsonParser::parseJSON(void) {
    _itr2 = _content.begin();
    std::cout << "Parsing JSON" << std::endl;
    return parseValue();
}

/**
 * @brief Parse a JSON value from the JSON string. Lots of recursion occurs here.
 * 
 * @return JsonValue The parsed JSON value
*/
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

/**
 * @brief Parse a null from the JSON string
 * stringIterator _itr for nulls should start at 'n' and end at 'l'
 * then the next character should be ',' or '}' or EOF
 * 
 * @return JsonValue The parsed null value
*/
JsonValue JsonParser::parseNull(void) {
    _itr += 4; // move from 'n' to after 'l'

    if (*_itr != ',' && *_itr != '}')
        throw std::runtime_error(ERR_JSON_PARSE);

    JsonValue parsedValue;
    parsedValue.setNull();
    return parsedValue;
}

/**
 * @brief Parse a string from the JSON string
 * stringIterator _itr for strings should start at '\"' and end at '\"'
 * then the next character should be ',' or '}' or ':' or ']' or EOF
 * 
 * @return JsonValue The parsed string
*/
JsonValue JsonParser::parseString(void) {
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

    JsonValue parsedValue;
    parsedValue.setString(value);
    return parsedValue;
}

/**
 * @brief Parse a number from the JSON string
 * stringIterator _itr for numbers should start at a digit and end at a non-digit
 * then the next character should be ',' or '}' or ']' or EOF
 * 
 * @return JsonValue The parsed number
*/
JsonValue JsonParser::parseNumber(void) {
    stringIterator tempItr = _itr;

    while (_itr != _content.end() && isdigit(*_itr)) {
        if (*_itr == '.')
            throw std::runtime_error(ERR_JSON_TYPE);
        _itr++;
    }

    int number = std::atoi(std::string(tempItr, _itr).c_str());

    if (_itr == _content.end() || (*_itr != ',' && *_itr != '}' && *_itr != ']'))
        throw std::runtime_error(ERR_JSON_PARSE);

    JsonValue parsedValue;
    parsedValue.setNumber(number);
    return parsedValue;
}

/**
 * @brief Parse a boolean from the JSON string
 * stringIterator _itr for booleans should start at 't' or 'f' and end at 'e'
 * then the next character should be ',' or '}' or ']' or EOF
 * 
 * @return JsonValue The parsed boolean
*/
JsonValue JsonParser::parseBoolean(void) {
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

    JsonValue parsedValue;
    parsedValue.setBoolean(boolean);
    return parsedValue;
}

/**
 * @brief Parse an array from the JSON string
 * stringIterator _itr for arrays should start at '[' and end at ']'
 * then the next character should be ',' or '}' or '[' or EOF
 * 
 * @return JsonValue The parsed array
*/
JsonValue JsonParser::parseArray(void) {
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
    _itr++; // move from ',' or '}' or ']'

    JsonValue parsedValue;
    parsedValue.setArray(array);
    return parsedValue;
}

/**
 * @brief Parse an object from the JSON string
 * stringIterator _itr for objects should start at '{' and end at '}'
 * then the next character should be ',' or '}' or '[' or EOF
 * 
 * @return JsonValue The parsed object
*/
JsonValue JsonParser::parseObject(void) {
    _itr++; // move from '{'

    std::map<std::string, JsonValue> objectMap;

    while (_itr != _content.end() && *_itr != '}') {
        if (*_itr != '\"')
            throw std::runtime_error(ERR_JSON_PARSE);

        std::string key = parseString().asString();
        _itr++; // move from ':'

        objectMap[key] = parseValue();

        if (*_itr != ',' && *_itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);

        if (*_itr == ',')
            _itr++; // move from ','
    }

    if (*(++_itr) != ',' && *_itr != ']' && _itr != _content.end())
        throw std::runtime_error(ERR_JSON_PARSE);

    JsonValue parsedValue;
    parsedValue.setObject(objectMap);
    return parsedValue;
}
