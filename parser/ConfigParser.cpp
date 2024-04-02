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

void ConfigParser::_validate_braces(void) {
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

void ConfigParser::_parse_error_page_directive(vector<ErrorPage>& errorPages) {
    DEBUG_MSG("Parsing error_page directive", RE);
}

vector<string> ConfigParser::_parse_index_directive(void) {
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

Location ConfigParser::_parse_location_context(void) {
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

fd ConfigParser::_parse_listen_directive(void) {
    DEBUG_MSG("Parsing listen directive", RE);

    while (*_itr != "}")
    {
        if (*_itr == "listen") {
            //! listen can also handle IP address like 0.0.0.0:80
            ++_itr; // move to port number
            if (*(_itr + 1) != ";")
                throw runtime_error(ERR_MISSING_SEMICOLON);

            if (!_isStringNumber(*_itr))
                throw runtime_error(ERR_INVALID_LISTEN);

}

void ConfigParser::_parse_server_name_directive(ServerConfig& serverConfig) {
    DEBUG_MSG("Parsing server_name directive", RE);

}

string ConfigParser::_parse_root_directive(void) {
    DEBUG_MSG("Parsing root directive", RE);
}

string ConfigParser::_parse_client_max_body_size_directive(void) {
    DEBUG_MSG("Parsing client_max_body_size directive", RE);
}

bool ConfigParser::_parse_autoindex_directive(void) {
    DEBUG_MSG("Parsing autoindex directive", RE);
}

ServerConfig ConfigParser::_parse_server_context(void) {
    DEBUG_MSG("Parsing server context", RE);
            cout << "ERROR: " << *_itr << endl;
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN_SERVER);
        }
        if (*_itr == ";")
            ++_itr;
    }
}

void ConfigParser::_parse_HTTP_context(void) {
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
            _serverConfigs.push_back(_parse_server_context());
        else
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN);
        ++_itr;
    }

    if ((_itr + 1) != _tokens.end())
        throw runtime_error(ERR_UNEXPECTED_TOKENS_OUT);
}

vector<ServerConfig> ConfigParser::parse(void) {
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

    _validate_braces();
    _parse_HTTP_context();

    return _serverConfigs;
}
