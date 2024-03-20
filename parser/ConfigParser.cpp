#include "ConfigParser.hpp"

ConfigParser::ConfigParser(std::string const& configFile) {
    // read file
    std::cout << "Parsing " << W << configFile << RE << std::endl;

    if (configFile.find(".conf") == std::string::npos) {
        throw std::runtime_error(ERR_INVALID_FILE);
        return;
    }

    struct stat fileStat;
    if (stat(configFile.c_str(), &fileStat) == -1 || !S_ISREG(fileStat.st_mode)) {
        throw std::runtime_error(ERR_STAT);
        return;
    }

    std::ifstream file(configFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error(ERR_OPEN);
        return;
    } else if (file.peek() == std::ifstream::traits_type::eof()) {
        throw std::runtime_error(ERR_EMPTY);
        return;
    }

    // get each line, remove comments and ignore empty lines
    std::string line;
    while (std::getline(file, line)) {
        std::string::size_type pos = line.find('#');
        if (pos != std::string::npos)
            line.erase(pos);
        if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
            continue;

        // remove tabs and spaces at the beginning and end of the line
        line.erase(0, line.find_first_not_of(" \t"));

        _content.append(line);
    }

    file.close();
}

void ConfigParser::parse(void) {
}
