#include "ConfigParser.hpp"

ConfigParser::ConfigParser(string const& configFile) {
    DEBUG_MSG("Parsing " << W << configFile, RE);

    // checking file extension
    if (configFile.find(".conf") == string::npos)
        throw runtime_error(ERR_INVALID_FILE);

    // check if file exists and is a regular file
    struct stat fileStat;
    if (stat(configFile.c_str(), &fileStat) == -1 || !S_ISREG(fileStat.st_mode))
        throw runtime_error(ERR_STAT);

    // open file
    ifstream file(configFile.c_str());
    if (!file.is_open())
        throw runtime_error(ERR_OPEN);
    else if (file.peek() == ifstream::traits_type::eof()) // check if file is empty
        throw runtime_error(ERR_EMPTY);

    string line;
    while (std::getline(file, line)) {
        // remove comments
        size_t pos = line.find('#');
        if (pos != string::npos)
            line.erase(pos);

        // ignore empty lines and lines with only tabs and spaces
        if (line.empty() || line.find_first_not_of(" \t") == string::npos)
            continue;

        // remove tabs and spaces at the beginning and end of the line
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        _content.append(line);
    }

    file.close();
}

Config ConfigParser::parse(void) {
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

    // check braces
    stack<string> braces;
    for (vector<string>::const_iterator itr = _tokens.begin();
         itr != _tokens.end(); ++itr)
    {
        if (*itr == "{")
            braces.push("{");
        else if (*itr == "}") {
            if (braces.empty()) // missing opening brace
                throw runtime_error(ERR_OPENINING_BRACE);
            braces.pop();
        }
    }
    if (!braces.empty()) // missing closing brace
        throw runtime_error(ERR_CLOSING_BRACE);

    // setting values for Config object
    for (vector<string>::const_iterator itr = _tokens.begin();
         itr != _tokens.end(); ++itr)
    {
        if (*itr == "listen") {
            ++itr; // move to port number

            if (*(itr + 1) != ";")
                throw runtime_error(ERR_MISSING_SEMICOLON);

            // check if port number is valid
            if (itr->find_first_not_of("0123456789") != string::npos)
                throw runtime_error("Parser: invalid port number");

            _config.listenerPort = std::atoi(itr->c_str());
            if (_config.listenerPort < 0 || _config.listenerPort > 65535)
                throw runtime_error("Parser: invalid port number");

            ++itr; // move to ;
        }
    }

    return _config;
}
