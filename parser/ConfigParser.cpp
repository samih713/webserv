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

void ConfigParser::_validateBraces(void) {
    DEBUG_MSG("Validating braces", RE);

    // check braces
    stack<string> braces;
    for (vector<string>::const_iterator itr = _tokens.begin();
         itr != _tokens.end(); ++itr)
    {
        if (*itr == "{") {
            if (*(itr - 1) != "server" && *(itr - 1) != "http" && *(itr - 2) != "location" && *(itr - 3) != "location")
                throw runtime_error(ERR_MISSING_CONTEXT);
            braces.push("{");
        }
        else if (*itr == "}") {
            if (braces.empty()) // missing opening brace
                throw runtime_error(ERR_OPENINING_BRACE);
            braces.pop();
        }
    }
    if (!braces.empty()) // missing closing brace
        throw runtime_error(ERR_CLOSING_BRACE);
}

void ConfigParser::_parseServerContext(void) {
    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_OPENINING_BRACE);

    while (*_itr != "}") {
        std::cout << *_itr << std::endl;
        if (*_itr == "listen") {
            ++_itr; // move to port number
            for (string::const_iterator portItr = _itr->begin(); portItr != _itr->end(); ++portItr) {
                if (!std::isdigit(*portItr))
                    throw runtime_error(ERR_INVALID_LISTEN);
            }
            _config.listenerPort = std::atoi(_itr->c_str());
            if (_config.listenerPort > MAX_PORT || _config.listenerPort < 0)
                throw runtime_error(ERR_INVALID_LISTEN);
        }
        else if (*_itr == "server_name") {
            ++_itr; // move to server name
            while (*_itr != ";") {
                _config.serverName.push_back(*_itr);
                ++_itr;
            }
        }
        else if (*_itr == "location") {
            // ++_itr; // move to location path
            // _config.locations.push_back(*_itr);
            while (*_itr != "}")
                ++_itr;
            ++_itr;
        }
        ++_itr;
    }
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

    _validateBraces();

    // setting values for Config object
    _itr = _tokens.begin();
    if (*_itr != "http")
        throw runtime_error(ERR_MISSING_HTTP);
    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_OPENINING_BRACE);

    while (_itr != _tokens.end()) {
        if (*_itr == "{")
            ++_itr;
        if (*_itr == "server")
            _parseServerContext();
        ++_itr;
    }

    return _config;
}
