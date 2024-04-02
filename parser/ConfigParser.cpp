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

void ConfigParser::_parseIndexDirective(void) {
    DEBUG_MSG("Parsing index directive", RE);

    ++_itr; // move to index file
    _config.home = _config.serverRoot;
    while (*_itr != ";") {
        if (_isKeyword(*_itr))
            throw runtime_error(ERR_INVALID_INDEX);
        _config.home += "/" + *_itr;
        ++_itr;
    }
}

void ConfigParser::_parseLocationContext(void) {
    DEBUG_MSG("Parsing location context", RE);

    ++_itr; // move to location modifier/path
    if (*_itr == "{")
        throw runtime_error(ERR_INVALID_LOCATION);
    if (*_itr == "=" || *_itr == "~") {
        _config.location.modifier = *_itr;
        ++_itr; // move to path
    }
    _config.location.path = *_itr;

    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_INVALID_LOCATION);
    ++_itr; // move to location content

    while (*_itr != "}") {
        if (*_itr == "root") {
            ++_itr; // move to next root path
            _config.location.root = *_itr;
            _checkSemicolon();
        } else if (*_itr == "index") {
            _parseIndexDirective();
        } else {
            cout << *_itr << endl;
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN_LOCATION);
        }
        ++_itr;
    }
    ++_itr; // move to next directive
}

void ConfigParser::_parseServerContext(void) {
    DEBUG_MSG("Parsing server context", RE);

    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_OPENINING_BRACE);
    ++_itr; // move to first directive

    while (*_itr != "}")
    {
        if (*_itr == "listen") {
            //! listen can also handle IP address like 0.0.0.0:80
            ++_itr; // move to port number
            if (*(_itr + 1) != ";")
                throw runtime_error(ERR_MISSING_SEMICOLON);

            if (!_isStringNumber(*_itr))
                throw runtime_error(ERR_INVALID_LISTEN);

            _config.listenerPort = std::atoi(_itr->c_str());
            if (_config.listenerPort > MAX_PORT || _config.listenerPort < 0)
                throw runtime_error(ERR_INVALID_LISTEN);
            ++_itr; // move to semicolon
        } else if (*_itr == "server_name") {
            ++_itr; // move to server name
            while (*_itr != ";") {
                if (_isKeyword(*_itr))
                    throw runtime_error(ERR_INVALID_SERVER_NAME);
                _config.serverName.push_back(*_itr);
                ++_itr;
            }
        } else if (*_itr == "error_page") {
            ++_itr; // move to error code
            while (_isStringNumber(*_itr)) {
                // _config.errorPages.codes.push_back(std::atoi(_itr->c_str()));
                ++_itr; // move to next error code
            }
            if (*_itr == ";")
                throw runtime_error(ERR_ERROR_PATH);

            // _config.errorPages.page = *_itr;
            _checkSemicolon();
        } else if (*_itr == "root") {
            ++_itr; // move to root path
            _config.serverRoot = *_itr;
            _checkSemicolon();
        } else if (*_itr == "index") {
            _parseIndexDirective();
        } else if (*_itr == "location") {
            _parseLocationContext();
        } else {
            cout << "ERROR: " << *_itr << endl;
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN_SERVER);
        }
        if (*_itr == ";")
            ++_itr;
    }
}

void ConfigParser::_parseHTTPContext(void) {
    DEBUG_MSG("Parsing HTTP context", RE);

    // setting values for Config object
    _itr = _tokens.begin();
    if (*_itr != "http") //! maybe there's stuff in the global context
        throw runtime_error(ERR_MISSING_HTTP);

    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_OPENINING_BRACE);
    ++_itr; // move to first directive

    while (_itr != _tokens.end()) {
        if (*_itr == "}")
            break;
        if (*_itr == "server")
            _parseServerContext();
        //! else if (*_itr == "index")
        else
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN);
        ++_itr;
    }

    if ((_itr + 1) != _tokens.end())
        throw runtime_error(ERR_UNEXPECTED_TOKENS_OUT);
}

Config ConfigParser::parse(void) {
    DEBUG_MSG("Tokenizing content", RE);
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
    _parseHTTPContext();

    return _config;
}
