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

void ConfigParser::_parse_error_page_directive(map<STATUS_CODE, string>& errorPages, string const& root) {
    DEBUG_MSG("Parsing error_page directive", RE);

    if (root.empty())
        throw runtime_error(ERR_MISSING_ROOT);

    ++_itr; // move to error code
    if (*_itr == ";")
        throw runtime_error(ERR_ERROR_CODE);

    vector<STATUS_CODE> codes;
    while (_isStringNumber(*_itr)) {
        codes.push_back(static_cast<STATUS_CODE>(std::atoi(_itr->c_str())));
        ++_itr; // move to next error code
    }
    if (*_itr == ";")
        throw runtime_error(ERR_ERROR_PATH);

    string errorPath = *_itr;
    if (errorPath.find(".html") == string::npos &&
        errorPath.find(".htm") == string::npos &&
        errorPath.find(".txt") == string::npos)
        throw runtime_error(ERR_INVALID_ERROR_PATH);
    if (errorPath.find("/") == string::npos)
        throw runtime_error(ERR_INVALID_ERROR_PATH);
    else if (errorPath.find_first_of("/") != errorPath.find_last_of("/"))
        throw runtime_error(ERR_INVALID_ERROR_PATH);
    if (errorPath.find("x") == string::npos) {
        if (errorPath.find_first_of("x") != errorPath.find_last_of("x"))
            throw runtime_error(ERR_INVALID_ERROR_PATH);
    }

    for (vector<STATUS_CODE>::const_iterator itr = codes.begin(); itr != codes.end(); ++itr) {
        string page = root + errorPath;
        size_t xPos = page.find("x");
        if (xPos != string::npos) {
            char lastDigit = *itr % 10 + '0';
            page.replace(xPos, 1, 1, lastDigit);
        }
        errorPages[*itr] = page;
    }

    _checkSemicolon();
}

vector<string> ConfigParser::_parse_index_directive(string const& root) {
    DEBUG_MSG("Parsing index directive", RE);
    vector<string> indexFiles;

    if (root.empty())
        throw runtime_error(ERR_MISSING_ROOT);

    ++_itr; // move to index file
    while (*_itr != ";") {
        if (_isKeyword(*_itr))
            throw runtime_error(ERR_INVALID_INDEX);
        indexFiles.push_back(root + "/" + *_itr);
        ++_itr;
    }

    return indexFiles;
}

Location ConfigParser::_parse_location_context(void) {
    DEBUG_MSG("Parsing location context", RE);
    Location location;

    ++_itr; // move to location modifier/path
    if (*_itr == "{")
        throw runtime_error(ERR_INVALID_LOCATION);
    if (*_itr == "=" || *_itr == "~") {
        location.modifier = *_itr;
        ++_itr; // move to path
    }
    location.path = *_itr;

    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_INVALID_LOCATION);
    ++_itr; // move to location content

    while (*_itr != "}") {
        if (*_itr == "root")
            location.root = _parse_root_directive();
        else if (*_itr == "autoindex")
            location.autoindex = _parse_autoindex_directive();
        else if (*_itr == "index")
            location.indexFiles = _parse_index_directive(location.root);
        else {
            cout << *_itr << endl;
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN_LOCATION);
        }
        ++_itr;
    }
    ++_itr; // move to next directive

    return location;
}

fd ConfigParser::_parse_listen_directive(void) {
    DEBUG_MSG("Parsing listen directive", RE);
    fd listenerPort;

    ++_itr; // move to port number
    if (*(_itr + 1) != ";")
        throw runtime_error(ERR_MISSING_SEMICOLON);

    if (!_isStringNumber(*_itr))
        throw runtime_error(ERR_INVALID_LISTEN);

    listenerPort = std::atoi(_itr->c_str());
    if (listenerPort > MAX_PORT || listenerPort < 0)
        throw runtime_error(ERR_INVALID_LISTEN);
    ++_itr; // move to semicolon

    //! listen can also handle IP address like 0.0.0.0:80
    return listenerPort;
}

void ConfigParser::_parse_server_name_directive(vector<string>& serverName) {
    DEBUG_MSG("Parsing server_name directive", RE);

    ++_itr; // move to server name
    while (*_itr != ";") {
        if (_isKeyword(*_itr))
            throw runtime_error(ERR_INVALID_SERVER_NAME);
        serverName.push_back(*_itr);
        ++_itr;
    }
}

string ConfigParser::_parse_root_directive(void) {
    DEBUG_MSG("Parsing root directive", RE);

    ++_itr; // move to root path
    if (*(_itr + 1) != ";")
        throw runtime_error(ERR_MISSING_SEMICOLON);
    string rootPath = *_itr;
    _checkSemicolon();
    return rootPath;
}

string ConfigParser::_parse_client_max_body_size_directive(void) {
    DEBUG_MSG("Parsing client_max_body_size directive", RE);

    ++_itr; // move to max body size
    if (*(_itr + 1) != ";")
        throw runtime_error(ERR_MISSING_SEMICOLON);
    string maxBodySize = *_itr;
    //! need to handle suffixes like 1m, 1k, 1g
    if (maxBodySize.find_first_not_of("0123456789kKmMgG") != string::npos)
        throw runtime_error(ERR_INVALID_BODY_SIZE);
    else if (maxBodySize.find_first_of("0123456789") == string::npos)
        throw runtime_error(ERR_INVALID_BODY_SIZE);
    _checkSemicolon();
    return maxBodySize;
}

bool ConfigParser::_parse_autoindex_directive(void) {
    DEBUG_MSG("Parsing autoindex directive", RE);

    ++_itr; // move to on/off
    if (*_itr != "on" && *_itr != "off")
        throw runtime_error(ERR_INVALID_AUTOINDEX);
    _checkSemicolon();
    return *(_itr - 1) == "on";
}

ServerConfig ConfigParser::_parse_server_context(void) {
    DEBUG_MSG("Parsing server context", RE);

    ServerConfig _serverConfig;

    ++_itr; // move to {
    if (*_itr != "{")
        throw runtime_error(ERR_OPENINING_BRACE);
    ++_itr; // move to first directive

    while (*_itr != "}")
    {
        if (*_itr == "listen")
            _serverConfig.listenerPort = _parse_listen_directive();
        else if (*_itr == "server_name")
            _parse_server_name_directive(_serverConfig.serverName);
        else if (*_itr == "error_page")
            _parse_error_page_directive(_serverConfig.errorPages, _serverConfig.serverRoot);
        else if (*_itr == "root")
            _serverConfig.serverRoot = _parse_root_directive();
        else if (*_itr == "client_max_body_size")
            _serverConfig.maxBodySize = _parse_client_max_body_size_directive();
        else if (*_itr == "index")
            _serverConfig.indexFiles =_parse_index_directive(_serverConfig.serverRoot);
        else if (*_itr == "location")
            _serverConfig.locations.push_back(_parse_location_context());
        else if (*_itr == "autoindex")
            _serverConfig.autoindex = _parse_autoindex_directive();
        else
            throw runtime_error(ERR_UNEXPECTED_TOKENS_IN_SERVER);
        if (*_itr == ";")
            ++_itr;
    }

    return _serverConfig;
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
