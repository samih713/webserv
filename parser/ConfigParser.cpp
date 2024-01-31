#include "ConfigParser.hpp"

using namespace ConfigParser;

JsonValue ConfigParser::parseJSON(const std::string file) {
    std::string json;
    readFile(file, json);

    stringIterator itr = json.begin();
    if (*itr != '{')
        throw std::runtime_error(ERR_JSON_PARSE);
    return extractJSON(json, ++itr);
}

/**
 * @brief Read the JSON file, remove all ( \t\n\r) characters from each line
 * and store the content in a string
 * 
 * @param filepath
 * @param content string to store the content of the file
 * 
 * @throw std::runtime_error if the file cannot be opened
*/
void ConfigParser::readFile(const std::string& filepath, std::string& content) {
    std::cout << "Reading config file: " << filepath << std::endl;

    std::ifstream inputFileStream(filepath.c_str());
    if (inputFileStream.fail())
        throw std::runtime_error(ERR_OPEN);

    std::string line;
    while (std::getline(inputFileStream, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.empty())
            continue;
        content.append(line);
    }
    inputFileStream.close();
}

JsonValue ConfigParser::extractJSON(const std::string& content, stringIterator& itr) {
    std::map<std::string, JsonValue>* _jsonMap = new std::map<std::string, JsonValue>;
    if (_jsonMap == NULL)
        throw std::runtime_error(ERR_MEMORY_ALLOC);

    do {
        KeyValuePair pair = parseKeyValuePair(content, itr);
        _jsonMap->insert(pair);
    } while (*itr == ',' && ++itr != content.end());

    return (JsonValue){ .jsonMap = _jsonMap };
}

KeyValuePair ConfigParser::parseKeyValuePair(const std::string& content, stringIterator& itr) {
    if (itr == content.end())
        throw std::runtime_error(ERR_JSON_PARSE);

    std::string key;
    stringIterator tempItr;
    if (*itr == '\"') {
        tempItr = ++itr;
        while (*itr != '\"')
            ++itr;
        key = std::string(tempItr, itr);
        if (*(++itr) != ':')
            throw std::runtime_error(ERR_JSON_PARSE);
        itr++;
    }

    JsonValue value; // value can be string, number(int), null, array or object
    if ((*itr) == '\"' && isalpha(*(++itr))) {
        tempItr = itr;
        while (*itr != '\"')
            ++itr;
        value.string = new std::string(tempItr, itr);
        if (*(++itr) != ',' && *itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
    }
    else if (isdigit(*itr) || (*itr == '-' && isdigit(*(++itr)))) {
        tempItr = itr;
        while (isdigit(*itr)) {
            if (*itr == '.')
                throw std::runtime_error(ERR_JSON_TYPE);
            ++itr;
        }
        value.number = std::atoi(std::string(tempItr, itr).c_str());
        if (*itr != ',' && *itr != '}' && *itr != ']')
            throw std::runtime_error(ERR_JSON_PARSE);
    }
    else if (std::string(itr, itr + 4) == "null") {
        value.string = NULL;
        itr += 4;
        if (*itr != ',' && *itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
    }
    else if (std::string(itr, itr + 5) == "false") {
        value.boolean = false;
        itr += 5;
        if (*itr != ',' && *itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
    }
    else if (std::string(itr, itr + 4) == "true") {
        value.boolean = true;
        itr += 4;
        if (*itr != ',' && *itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
    }
    else if ((*itr) == '[') {
        value.array = new std::vector<JsonValue>;
        itr++;
        while (itr != content.end() && *itr != ']') {
            value.array->push_back(extractJSON(content, itr));
            if (*itr != ',' && *itr != ']')
                throw std::runtime_error(ERR_JSON_PARSE);
            if (*itr == ',')
                itr++;
        }
        if (*itr != ']' && (*(++itr) != ',' || *itr != '}'))
            throw std::runtime_error(ERR_JSON_PARSE);
        itr++;
    }
    else if (*itr == '{') {
        value = extractJSON(content, ++itr);
        if (*itr != ',' && *itr != '}')
            throw std::runtime_error(ERR_JSON_PARSE);
    }
    else
        throw std::runtime_error(ERR_JSON_PARSE);
    return std::make_pair(key, value);
}
