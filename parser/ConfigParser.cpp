#include "ConfigParser.hpp"

ConfigParser::ConfigParser() {}

ConfigParser::~ConfigParser() {}

void ConfigParser::parseJSON(std::string file) {
    std::cout << "Parsing config file: " << file << std::endl;

    std::ifstream inputFileStream(file);
    if (inputFileStream.fail()) {
        std::cerr << ERR_OPEN << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(inputFileStream, line)) {
        std::cout << line << std::endl;
    }
    inputFileStream.close();
}