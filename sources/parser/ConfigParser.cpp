#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const string& configFile)
{
    // checking file extension
    if (configFile.find('.') == string::npos ||
        configFile.substr(configFile.find_last_of('.')) != ".conf")
        THROW_EXCEPTION_WITH_INFO(ERR_FILE_EXTENSION);

    // check if file exists and is a regular file
    if (get_file_type(configFile) != REG_FILE)
        THROW_EXCEPTION_WITH_INFO(ERR_FILE + configFile);

    // open file
    ifstream file(configFile.c_str());
    if (!file.is_open())
        THROW_EXCEPTION_WITH_INFO(ERR_OPEN);
    if (file.peek() == ifstream::traits_type::eof()) // check if file is empty
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

    // check braces
    stack<string> braces;
    for (vector<string>::const_iterator vecItr = _tokens.begin(); vecItr != _tokens.end();
         ++vecItr)
    {
        if (*vecItr == "{") {
            if (vecItr - 1 >= _tokens.begin() && *(vecItr - 1) != "server" &&
                vecItr - 2 >= _tokens.begin() && *(vecItr - 2) != "location") //!
            {
                THROW_EXCEPTION_WITH_INFO(ERR_MISSING_CONTEXT);
            }
            braces.push("{");
        }
        else if (*vecItr == "}") {
            if (braces.empty()) // missing opening brace
                THROW_EXCEPTION_WITH_INFO(ERR_OPENING_BRACE);
            braces.pop();
        }
    }
    if (!braces.empty()) // missing closing brace
        THROW_EXCEPTION_WITH_INFO(ERR_CLOSING_BRACE);
}

void ConfigParser::parse_error_page(map<STATUS_CODE, string>& errorPages,
    const string&                                             root)
{
    if (root.empty()) // root can be empty if not set
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);

    ++_itr; // move to error code
    vector<STATUS_CODE> codes;
    while (is_number(*_itr)) {
        codes.push_back(static_cast<STATUS_CODE>(std::atoi(_itr->c_str())));
        ++_itr; // move to next error code
    }
    if (*_itr == ";")
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);

    string errorPath = *_itr;
    if (errorPath.find(".html") == string::npos &&
        errorPath.find(".htm") == string::npos && errorPath.find(".txt") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    if (errorPath.find("/") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    else if (errorPath.find_first_of("/") != errorPath.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    if (errorPath.find("x") == string::npos) {
        if (errorPath.find_first_of("x") != errorPath.find_last_of("x"))
            THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
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

    check_semicolon();
}

void ConfigParser::parse_index(string& indexFile, const string& root)
{
    if (root.empty()) // root can be empty if not set
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);

    ++_itr; // move to index file
    if (is_keyword(*_itr))
        THROW_EXCEPTION_WITH_INFO(ERR_INDEX);

    indexFile = root + "/" + *_itr;
    //! removed index file checking

    check_semicolon();
}

fd ConfigParser::parse_listen(in_addr_t& host, bool& defaultServer)
{
    ++_itr; // move to host:port

    string portStr;
    string hostStr;
    size_t colonPos = _itr->find(":");
    if (colonPos != string::npos) { // host:port
        if (colonPos == 0)
            THROW_EXCEPTION_WITH_INFO(ERR_HOST);
        hostStr = _itr->substr(0, colonPos);
        portStr = _itr->substr(colonPos + 1);
    }
    else if (_itr->find(".") != string::npos)
        hostStr = *_itr;
    else if (is_number(*_itr))
        portStr = *_itr;
    else if (_itr->find_first_of(":") != _itr->find_last_of(":"))
        THROW_EXCEPTION_WITH_INFO(ERR_MULTIPLE_COLON);
    else
        THROW_EXCEPTION_WITH_INFO(ERR_LISTEN);

    // parse host
    if (!hostStr.empty()) {
        if (hostStr == "localhost" || hostStr == "127.0.0.1")
            host = inet_addr("127.0.0.1");
        else {
            struct sockaddr_in sockaddr;
            if (inet_pton(AF_INET, hostStr.c_str(), &sockaddr.sin_addr))
                host = inet_addr(hostStr.c_str());
            else
                THROW_EXCEPTION_WITH_INFO(ERR_HOST);
        }
    }

    // parse port
    fd port = 8080;
    if (!portStr.empty()) {
        port = (int) std::strtod(portStr.c_str(), NULL);
        if (port > MAX_PORT || port < 0)
            THROW_EXCEPTION_WITH_INFO(ERR_PORT);
    }

    if (*(_itr + 1) == "default_server") {
        defaultServer = true;
        ++_itr; // move to default_server
    }

    check_semicolon();

    return port;
}

void ConfigParser::parse_server_name(string& serverName)
{
    ++_itr; // move to server name
    serverName = *_itr;
    if (is_keyword(serverName))
        THROW_EXCEPTION_WITH_INFO(ERR_SERVER_NAME);
    check_semicolon();
}

void ConfigParser::parse_root(string& root)
{
    ++_itr; // move to root path
    root = *_itr;
    if (root[root.size() - 1] == '/') // remove trailing slash
        root.erase(root.size() - 1);
    if (get_file_type(root) != DIR) //!
        THROW_EXCEPTION_WITH_INFO(ERR_ROOT);
    check_semicolon();
}

size_t ConfigParser::parse_client_max_body_size(void)
{
    ++_itr; // move to max body size

    string maxBodySize = *_itr;
    if (maxBodySize.find_first_not_of("0123456789kKmMgG.") != string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_CHAR);
    else if (maxBodySize.find_first_of("0123456789") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SIZE);
    else if (maxBodySize.find_first_of("kKmMgG") == string::npos) {
        if (maxBodySize == "0") {
            check_semicolon();
            return 0; // disable body size checking
        }
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SUFFIX);
    }
    else if (maxBodySize.find_first_of(".") != maxBodySize.find_last_of("."))
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_CHAR);

    // get the suffix and its associated value
    string suffix = maxBodySize.substr(maxBodySize.find_first_not_of("0123456789."));
    if (suffix.size() > 1)
        THROW_EXCEPTION_WITH_INFO(ERR_MULTIPLE_SUFFIX);

    size_t multiplier = 1;
    if (suffix == "k" || suffix == "K")
        multiplier = 1000;
    else if (suffix == "m" || suffix == "M")
        multiplier = 1000000;
    else if (suffix == "g" || suffix == "G")
        multiplier = 1000000000;

    string value = maxBodySize.substr(0, maxBodySize.find_first_not_of("0123456789."));
    double size  = std::strtod(value.c_str(), NULL);
    if (size >= std::numeric_limits<int>::max())
        THROW_EXCEPTION_WITH_INFO(ERR_BODY_SIZE_OVERFLOW);

    check_semicolon();

    return size * multiplier;
}

bool ConfigParser::parse_autoindex(void)
{
    ++_itr; // move to on/off
    string autoindex = *_itr;
    if (autoindex != "on" && autoindex != "off")
        THROW_EXCEPTION_WITH_INFO(ERR_AUTOINDEX);
    check_semicolon();
    return autoindex == "on";
}

void ConfigParser::parse_allow_methods(vector<string>& methods)
{
    ++_itr; // move to methods

    while (*_itr != ";") {
        if (*_itr == "GET" || *_itr == "POST" || *_itr == "DELETE" ||
            *_itr == "PUT") // add more methods here
            methods.push_back(*_itr);
        else
            THROW_EXCEPTION_WITH_INFO(ERR_METHOD);
        ++_itr;
    }

    if (methods.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_EMPTY_METHODS);

    --_itr; // move back to last method
    check_semicolon();
}

void ConfigParser::parse_location_context(ServerConfig& server)
{
    ++_itr; // move to location uri

    string uri;
    uri = *_itr;
    if (uri[0] != '/') // uri should begin with /
        THROW_EXCEPTION_WITH_INFO(ERR_URI_MISSING_SLASH);
    else if (uri.find_first_of("/") != uri.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_URI_DUPLICATE_SLASH);

    if (server.locations.find(uri) != server.locations.end())
        THROW_EXCEPTION_WITH_INFO(ERR_DUPLICATE_LOCATION);

    ++_itr; // move to {

    Location location;
    location.root        = server.root;
    location.indexFile   = server.indexFile;
    location.autoindex   = server.autoindex;
    location.maxBodySize = server.maxBodySize;

    set<string> parsedDirectives;
    while (*_itr != "}") {
        if (*_itr != ";" && *_itr != "}")
            check_duplicate_element(parsedDirectives, *_itr, "location");
        if (*_itr == "root")
            parse_root(location.root);
        else if (*_itr == "client_max_body_size")
            location.maxBodySize = parse_client_max_body_size();
        else if (*_itr == "allow_methods")
            parse_allow_methods(location.methods);
        else if (*_itr == "index") {
            if (get_file_type(location.root + uri) != DIR) //!
                THROW_EXCEPTION_WITH_INFO(ERR_LOCATION_INDEX);
            parse_index(location.indexFile, location.root + uri);
        }
        else if (*_itr == "autoindex") {
            if (uri == "/cgi-bin") //?
                THROW_EXCEPTION_WITH_INFO(ERR_AUTOINDEX_CGI);
            location.autoindex = parse_autoindex();
        }
        else if (*_itr == ";" || *_itr == "{")
            ++_itr;
        else
            THROW_EXCEPTION_WITH_INFO(ERR_LOCATION_TOKENS);
    }

    if (parsedDirectives.find("root") == parsedDirectives.end() && location.root.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);
    else if (parsedDirectives.find("allow_methods") == parsedDirectives.end())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_METHODS);

    server.locations[uri] = location;

    ++_itr; // move to next directive
}

ServerConfig ConfigParser::parse_server_context(void)
{
    ++_itr; // move to {

    set<string>  parsedDirectives;
    ServerConfig server;
    while (*_itr != "}") {
        if (*_itr != ";" && *_itr != "}" && *_itr != "error_page" && *_itr != "location")
            check_duplicate_element(parsedDirectives, *_itr, "server");
        if (*_itr == "listen")
            server.port = parse_listen(server.host, server.defaultServer);
        else if (*_itr == "root")
            parse_root(server.root);
        else if (*_itr == "client_max_body_size")
            server.maxBodySize = parse_client_max_body_size();
        else if (*_itr == "autoindex")
            server.autoindex = parse_autoindex();
        else if (*_itr == "index")
            parse_index(server.indexFile, server.root);
        else if (*_itr == "server_name")
            parse_server_name(server.serverName);
        else if (*_itr == "error_page")
            parse_error_page(server.errorPages, server.root);
        else if (*_itr == "location")
            parse_location_context(server);
        else if (*_itr == ";" || *_itr == "{")
            ++_itr;
        else
            THROW_EXCEPTION_WITH_INFO(ERR_SERVER_TOKENS);
    }

    if (server.locations.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_LOCATION);

    ++_itr; // move to next context

    return server;
}

vector<ServerConfig> ConfigParser::parse(void)
{
    _itr = _tokens.begin(); // setting iterator to the first token

    vector<ServerConfig> servers;
    bool                 foundDefaultServer = false;
    while (_itr != _tokens.end()) {
        if (*_itr == "server")
            servers.push_back(parse_server_context());
        else
            THROW_EXCEPTION_WITH_INFO(ERR_TOKENS);

        if (servers[servers.size() - 1].defaultServer == true) {
            if (foundDefaultServer == true)
                THROW_EXCEPTION_WITH_INFO(ERR_MULTIPLE_DEFAULT);
            foundDefaultServer = true;
        }
    }

    if (servers.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SERVER);

    if (foundDefaultServer == false)
        servers[0].defaultServer = true;

    return servers;
}
