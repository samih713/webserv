#include "ConfigParser.hpp"

ConfigParser::ConfigParser(string const& configFile) {
    DEBUG_MSG("Parsing " << W << configFile, RE);

    // checking file extension
    if (configFile.find(".conf") == string::npos)
        throw std::runtime_error(ERR_INVALID_FILE);

    // check if file exists and is a regular file
    struct stat fileStat;
    if (stat(configFile.c_str(), &fileStat) == -1 || !S_ISREG(fileStat.st_mode))
        throw std::runtime_error(ERR_STAT);

    // open file
    ifstream file(configFile.c_str());
    if (!file.is_open())
        throw std::runtime_error(ERR_OPEN);
    else if (file.peek() == ifstream::traits_type::eof()) // check if file is empty
        throw std::runtime_error(ERR_EMPTY);

    // get each line, remove comments and ignore empty lines
    string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('#');
        if (pos != string::npos)
            line.erase(pos);
        if (line.empty() || line.find_first_not_of(" \t") == string::npos)
            continue;

        // remove tabs and spaces at the beginning and end of the line
        line.erase(0, line.find_first_not_of(" \t"));

        _content.append(line);
    }

    file.close();
}

void ConfigParser::_validate(void) {
    // check braces
    stack<string> braces;
    for (vector<string>::const_iterator itr = _tokens.begin();
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
    // tokenizing content
    string currentToken;
    for (string::const_iterator itr = _content.begin(); itr != _content.end(); ++itr) {
        if (*itr == '{' || *itr == '}' || *itr == ';') { // delimiters
            if (!currentToken.empty()) {
                _tokens.push_back(currentToken);
                currentToken.clear();
            }
            _tokens.push_back(string(1, *itr));
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

    _validate();
}
