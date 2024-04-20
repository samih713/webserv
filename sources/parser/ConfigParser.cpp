#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const string& configFile)
{
    // checking file extension
    if (configFile.find(".conf") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_FILE);

    // check if file exists and is a regular file
    struct stat fileStat;
    if (stat(configFile.c_str(), &fileStat) == -1 || !S_ISREG(fileStat.st_mode))
        THROW_EXCEPTION_WITH_INFO(ERR_STAT);

    // open file
    ifstream file(configFile.c_str());
    if (!file.is_open())
        THROW_EXCEPTION_WITH_INFO(ERR_OPEN);
    else if (file.peek() == ifstream::traits_type::eof()) // check if file is empty
        THROW_EXCEPTION_WITH_INFO(ERR_EMPTY);

    string line;
    string content;
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

        content.append(line);
    }

    file.close();

    string currentToken;
    for (string::const_iterator itr = content.begin(); itr != content.end(); ++itr) {
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
}

void ConfigParser::_parse_error_page(map<STATUS_CODE, string>& errorPages,
    const string&                                              root)
{
    if (root.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);

    ++_itr; // move to error code
    if (*_itr == ";")
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_CODE);

    vector<STATUS_CODE> codes;
    while (_is_string_number(*_itr)) {
        codes.push_back(static_cast<STATUS_CODE>(std::atoi(_itr->c_str())));
        ++_itr; // move to next error code
    }
    if (*_itr == ";")
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);

    string errorPath = *_itr;
    if (errorPath.find(".html") == string::npos &&
        errorPath.find(".htm") == string::npos && errorPath.find(".txt") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_ERROR_PATH);
    if (errorPath.find("/") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_ERROR_PATH);
    else if (errorPath.find_first_of("/") != errorPath.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_ERROR_PATH);
    if (errorPath.find("x") == string::npos) {
        if (errorPath.find_first_of("x") != errorPath.find_last_of("x"))
            THROW_EXCEPTION_WITH_INFO(ERR_INVALID_ERROR_PATH);
    }

    for (vector<STATUS_CODE>::const_iterator itr = codes.begin(); itr != codes.end();
         ++itr)
    {
        string page = root + errorPath;
        size_t xPos = page.find("x");
        if (xPos != string::npos) {
            char lastDigit = *itr % 10 + '0';
            page.replace(xPos, 1, 1, lastDigit);
        }
        errorPages[*itr] = page;
    }

    _check_semicolon();
}

vector<string> ConfigParser::_parse_index(const string& root)
{
    vector<string> indexFiles;

    if (root.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);

    ++_itr; // move to index file
    while (*_itr != ";") {
        if (_is_keyword(*_itr))
            THROW_EXCEPTION_WITH_INFO(ERR_INVALID_INDEX);
        indexFiles.push_back(root + "/" + *_itr);
        ++_itr;
    }

    return indexFiles;
}

Location ConfigParser::_parse_location_context(void)
{
    ++_itr; // move to location modifier/path
    if (*_itr == "{")
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_LOCATION);

    Location location;
    if (*_itr == "=" || *_itr == "~") {
        location.modifier = *_itr;
        ++_itr; // move to path
    }
    location.path = *_itr;

    ++_itr; // move to {
    if (*_itr != "{")
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_LOCATION);
    ++_itr; // move to location content

    while (*_itr != "}") {
        if (*_itr == "root")
            location.root = _parse_root();
        else if (*_itr == "autoindex")
            location.autoindex = _parse_autoindex();
        else if (*_itr == "index")
            location.indexFiles = _parse_index(location.root);
        else {
            cout << *_itr << endl;
            THROW_EXCEPTION_WITH_INFO(ERR_UNEXPECTED_TOKENS_IN_LOCATION);
        }
        ++_itr;
    }
    ++_itr; // move to next directive

    return location;
}

fd ConfigParser::_parse_listen(void)
{
    ++_itr; // move to port number
    if (*(_itr + 1) != ";")
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SEMICOLON);

    if (!_is_string_number(*_itr))
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_LISTEN);

    fd listenerPort =
        std::atoi(_itr->c_str()); //! listen can also handle IP address like 0.0.0.0:80
    if (listenerPort > MAX_PORT || listenerPort < 0)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_LISTEN);

    _check_semicolon();
    return listenerPort;
}

void ConfigParser::_parse_server_name(string& serverName)
{
    ++_itr; // move to server name
    if (_is_keyword(*_itr))
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_SERVER_NAME);
    serverName = *_itr;
    ++_itr;
}

string ConfigParser::_parse_root(void)
{
    ++_itr; // move to root path
    if (*(_itr + 1) != ";")
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SEMICOLON);

    string rootPath = *_itr;
    _check_semicolon();

    return rootPath;
}

size_t ConfigParser::_parse_client_max_body_size(void)
{
    ++_itr; // move to max body size
    if (*(_itr + 1) != ";")
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SEMICOLON);

    string maxBodySize = *_itr;
    if (maxBodySize.find_first_not_of("0123456789kKmMgG.") != string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_CHAR);
    else if (maxBodySize.find_first_of("0123456789") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SIZE);
    else if (maxBodySize.find_first_of("kKmMgG") == string::npos) {
        if (maxBodySize == "0") {
            _check_semicolon();
            return 0; // disable body size checking
        }
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SUFFIX);
    }
    else if (maxBodySize.find_first_of(".") != maxBodySize.find_last_of("."))
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_CHAR);

    // get the suffix and its associated value
    string suffix = maxBodySize.substr(maxBodySize.find_first_not_of("0123456789."));
    if (suffix.size() > 1)
        THROW_EXCEPTION_WITH_INFO(ERR_MULTIPLE_SIZE_SUFFIX);

    size_t multiplier = 1;
    if (suffix == "k" || suffix == "K")
        multiplier = 1000;
    else if (suffix == "m" || suffix == "M")
        multiplier = 1000000;
    else if (suffix == "g" || suffix == "G")
        multiplier = 1000000000;

    string value = maxBodySize.substr(0, maxBodySize.find_first_not_of("0123456789."));
    if (value.size() > 13)
        THROW_EXCEPTION_WITH_INFO(ERR_BODY_SIZE_OVERFLOW);

    double size = std::strtod(value.c_str(), NULL);

    _check_semicolon();

    return size * multiplier;
}

bool ConfigParser::_parse_autoindex(void)
{
    ++_itr; // move to on/off
    string autoindex = *_itr;
    if (autoindex != "on" && autoindex != "off")
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_AUTOINDEX);
    _check_semicolon();
    return autoindex == "on";
}

ServerConfig ConfigParser::_parse_server_context(void)
{
    ++_itr; // move to {
    if (*_itr != "{")
        THROW_EXCEPTION_WITH_INFO(ERR_OPENINING_BRACE);
    ++_itr; // move to first directive

    set<string>  parsedServerDirectives;
    ServerConfig _serverConfig;
    while (*_itr != "}") {
        if (*_itr == "listen") {
            _serverConfig.listenerPort = _parse_listen();
            check_duplicate_directive(parsedServerDirectives, "listen");
        }
        else if (*_itr == "server_name")
            _parse_server_name(_serverConfig.serverName);
        else if (*_itr == "error_page")
            _parse_error_page(_serverConfig.errorPages, _serverConfig.serverRoot);
        else if (*_itr == "root") {
            _serverConfig.serverRoot = _parse_root();
            check_duplicate_directive(parsedServerDirectives, "root");
        }
        else if (*_itr == "client_max_body_size") {
            _serverConfig.maxBodySize = _parse_client_max_body_size();
            check_duplicate_directive(parsedServerDirectives, "client_max_body_size");
        }
        else if (*_itr == "index")
            _serverConfig.indexFiles = _parse_index(_serverConfig.serverRoot);
        else if (*_itr == "location")
            _serverConfig.locations.push_back(_parse_location_context());
        else if (*_itr == "autoindex") {
            _serverConfig.autoindex = _parse_autoindex();
            check_duplicate_directive(parsedServerDirectives, "autoindex");
        }
        else
            THROW_EXCEPTION_WITH_INFO(ERR_UNEXPECTED_TOKENS_IN_SERVER);
        if (*_itr == ";")
            ++_itr;
    }

    return _serverConfig;
}

vector<ServerConfig> ConfigParser::_parse_HTTP_context(void)
{
    // setting values for Config object
    _itr = _tokens.begin();
    if (*_itr != "http") //! maybe there's stuff in the global context
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_HTTP);

    ++_itr; // move to {
    if (*_itr != "{")
        THROW_EXCEPTION_WITH_INFO(ERR_OPENINING_BRACE);
    ++_itr; // move to first directive

    vector<ServerConfig> serverConfigs;
    while (*_itr != "}") {
        if (*_itr == "server")
            serverConfigs.push_back(_parse_server_context());
        else
            THROW_EXCEPTION_WITH_INFO(ERR_UNEXPECTED_TOKENS_IN);
        ++_itr;
    }

    if ((_itr + 1) != _tokens.end())
        THROW_EXCEPTION_WITH_INFO(ERR_UNEXPECTED_TOKENS_OUT);

    return serverConfigs;
}

vector<ServerConfig> ConfigParser::parse(void)
{
    // check braces
    stack<string> braces;
    for (vector<string>::const_iterator itr = _tokens.begin(); itr != _tokens.end();
         ++itr)
    {
        if (*itr == "{") {
            if (*(itr - 1) != "server" && *(itr - 1) != "http" &&
                *(itr - 2) != "location" && *(itr - 3) != "location")
                THROW_EXCEPTION_WITH_INFO(ERR_MISSING_CONTEXT);
            braces.push("{");
        }
        else if (*itr == "}") {
            if (braces.empty()) // missing opening brace
                THROW_EXCEPTION_WITH_INFO(ERR_OPENINING_BRACE);
            braces.pop();
        }
    }
    if (!braces.empty()) // missing closing brace
        THROW_EXCEPTION_WITH_INFO(ERR_CLOSING_BRACE);

    return _parse_HTTP_context();
}
