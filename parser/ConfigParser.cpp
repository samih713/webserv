#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

void ConfigParser::parseJSON(const std::string file) {
    std::string json;
    readFile(file, json);
    std::cout << json;
}

void ConfigParser::readFile(const std::string& filepath, std::string& output) {
    std::cout << "Reading config file: " << filepath << std::endl;

    std::ifstream inputFileStream(filepath.c_str());
    if (inputFileStream.fail()) {
        std::cerr << ERR_OPEN << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(inputFileStream, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        if (line.empty())
            continue;
        output.append(line);
    }
    inputFileStream.close();
}
