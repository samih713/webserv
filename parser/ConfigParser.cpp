#include "ConfigParser.hpp"

using namespace ConfigParser;

JsonValue ConfigParser::parseJSON(const std::string file) {
    std::string json;
    readFile(file, json);

    stringIterator itr = json.begin();
    KeyValuePair test = parseKeyValuePair(json, ++itr);

    std::cout << "Key: " << test.first << "$" << std::endl;
    std::cout << "Value: " << test.second.number << std::endl;
}

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
    else
        throw std::runtime_error(ERR_JSON_PARSE);
    return std::make_pair(key, value);
}