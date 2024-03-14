#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string filepath) {
    std::cout << "Reading config file: " << filepath << std::endl;

    struct stat filestat;
    if (stat(filepath.c_str(), &filestat) != 0)
        throw std::runtime_error(ERR_OPEN);
    if (!S_ISREG(filestat.st_mode))
        throw std::runtime_error(ERR_NOT_REGULAR);

    std::ifstream inputFileStream(filepath.c_str());
    if (inputFileStream.fail())
        throw std::runtime_error(ERR_OPEN);

    // Reading the entire file into a string, then removing all whitespace
    std::string file((std::istreambuf_iterator<char>(inputFileStream)), std::istreambuf_iterator<char>());
    file.erase(remove_if(file.begin(), file.end(), isspace), file.end());

    _content = file;

    inputFileStream.close();
}

JsonValue ConfigParser::parseConfig(void) {
    JsonValue config = JsonParser(_content).parseJSON();
    return config;
}