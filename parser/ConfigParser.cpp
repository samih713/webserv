#include "ConfigParser.hpp"

ConfigParser::ConfigParser(std::string const& configFile) {
    // read file
    std::cout << "Parsing " << W << configFile << RE << std::endl;

    // checking file extension
    if (configFile.find(".conf") == std::string::npos) {
        throw std::runtime_error(ERR_INVALID_FILE);
        return;
    }

    // check if file exists and is a regular file
    struct stat fileStat;
    if (stat(configFile.c_str(), &fileStat) == -1 || !S_ISREG(fileStat.st_mode)) {
        throw std::runtime_error(ERR_STAT);
        return;
    }

    // open file
    std::ifstream file(configFile.c_str());
    if (!file.is_open()) {
        throw std::runtime_error(ERR_OPEN);
        return;
    } else if (file.peek() == std::ifstream::traits_type::eof()) { // check if file is empty
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

void ConfigParser::_tokenize(void) {
    std::string currentToken;
    for (std::string::const_iterator itr = _content.begin(); itr != _content.end(); ++itr) {
        if (*itr == '{' || *itr == '}' || *itr == ';') { // delimiters
            if (!currentToken.empty()) {
                _tokens.push_back(currentToken);
                currentToken.clear();
            }
            _tokens.push_back(std::string(1, *itr));
        }
        else if (std::isspace(*itr)) { // whitespace
            if (!currentToken.empty()) {
                _tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
        else
            currentToken.push_back(*itr); // add character to current token
    }
}

void ConfigParser::_validate(void) {
    // check braces
    std::stack<std::string> braces;
    for (std::vector<std::string>::const_iterator itr = _tokens.begin();
         itr != _tokens.end(); ++itr)
    {
        if (*itr == "{")
            braces.push("{");
        else if (*itr == "}") {
            if (braces.empty()) // missing opening brace
                throw std::runtime_error(ERR_OPENINING_BRACE);
            braces.pop();
        }
    }
    if (!braces.empty()) // missing closing brace
        throw std::runtime_error(ERR_CLOSING_BRACE);

}

void ConfigParser::parse(void) {
    _tokenize();

    _validate();
}
