#include "parse_config.hpp"

/* ---------------------------- UTILITY FUNCTIONS --------------------------- */

static inline bool is_number(const string& str)
{
    if (str.find_first_not_of("0123456789") != string::npos)
        return false;
    return true;
}

static inline bool is_keyword(const string& str)
{
    const string keywords[11] = { "server", "listen", "server_name", "location", "root",
        "index", "error_page", "client_max_body_size", "autoindex", "allow_methods",
        "redirect" };
    for (int i = 0; i < 11; i++)
        if (keywords[i] == str)
            return true;
    return false;
}

static inline void check_semicolon(vector<string>::const_iterator& token)
{
    if (*(token + 1) != ";")
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SEMICOLON + *token);
    ++token; // move to semicolon
}

static inline void check_duplicate_element(set<string>& container, const string& element,
    const string& context)
{
    if (container.find(element) != container.end())
        THROW_EXCEPTION_WITH_INFO(
            "Config: duplicate " + element + " found in " + context + " context");

    container.insert(element);
}

/* ---------------------------- PARSING FUNCTIONS --------------------------- */

static string read_file(const string& filePath)
{
    // checking file extension
    if (filePath.find('.') == string::npos ||
        filePath.substr(filePath.find_last_of('.')) != ".conf")
        THROW_EXCEPTION_WITH_INFO(ERR_FILE_EXTENSION);

    // check if file exists and is a regular file
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) == -1)
        THROW_EXCEPTION_WITH_INFO(ERR_FILE + filePath);

    if (S_ISDIR(fileInfo.st_mode))
        THROW_EXCEPTION_WITH_INFO(ERR_FILE_TYPE);

    // check file permissions
    if (access(filePath.c_str(), R_OK | W_OK) == -1)
        THROW_EXCEPTION_WITH_INFO(ERR_FILE_PERM);

    // open file
    ifstream file(filePath.c_str());
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

    return content;
}

static vector<string> tokenize_content(const string& content)
{
    vector<string> tokens;
    string         currentToken;
    for (string::const_iterator itr = content.begin(); itr != content.end(); ++itr) {
        if (*itr == '{' || *itr == '}' || *itr == ';') { // delimiters
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            tokens.push_back(string(1, *itr));
        }
        else if (std::isspace(*itr)) { // whitespace
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
        else
            currentToken.push_back(*itr); // add character to current token
    }
    return tokens;
}

static void validate_braces(vector<string>& tokens)
{
    // check braces
    stack<string> braces;
    for (vector<string>::const_iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
    {
        if (*itr == "{") {
            if (itr - 1 >= tokens.begin() && *(itr - 1) != "server" &&
                itr - 2 >= tokens.begin() && *(itr - 2) != "location")
            {
                THROW_EXCEPTION_WITH_INFO(ERR_MISSING_CONTEXT);
            }
            braces.push("{");
        }
        else if (*itr == "}") {
            if (braces.empty()) // missing opening brace
                THROW_EXCEPTION_WITH_INFO(ERR_OPENING_BRACE);
            braces.pop();
        }
    }
    if (!braces.empty()) // missing closing brace
        THROW_EXCEPTION_WITH_INFO(ERR_CLOSING_BRACE);
}

/* ----------------------- PARSING DIRECTIVE FUNCTIONS ---------------------- */

static void parse_error_page(vector<string>::const_iterator& token,
    StatusCodeMap& errorPages, const string& root)
{
    if (root.empty()) // root can be empty if not set
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);

    ++token; // move to error code
    vector<STATUS_CODE> codes;
    while (is_number(*token)) {
        codes.push_back(static_cast<STATUS_CODE>(std::atoi(token->c_str())));
        ++token; // move to next error code
    }

    string errorFile = *token;
    if (errorFile.find(".html") == string::npos && errorFile.find(".htm") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    if (errorFile.find("/") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    else if (errorFile.find_first_of("/") != errorFile.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    if (errorFile.find("x") == string::npos) {
        if (errorFile.find_first_of("x") != errorFile.find_last_of("x"))
            THROW_EXCEPTION_WITH_INFO(ERR_ERROR_PATH);
    }

    for (vector<STATUS_CODE>::const_iterator itr = codes.begin(); itr != codes.end();
         ++itr)
    {
        string page = root + errorFile;
        size_t xPos = page.find("x");
        if (xPos != string::npos) {
            char lastDigit = *itr % 10 + '0';
            page.replace(xPos, 1, 1, lastDigit);
        }
        errorPages[*itr] = page;
    }

    check_semicolon(token);
}

static void parse_index(vector<string>::const_iterator& token, string& indexFile,
    const string& root)
{
    if (root.empty()) // root can be empty if not set
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);

    ++token; // move to index file
    if (is_keyword(*token) || token->find("/") != string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INDEX);

    indexFile = *token;

    check_semicolon(token);
}

static fd parse_listen(vector<string>::const_iterator& token, in_addr_t& host,
    bool& defaultServer)
{
    ++token; // move to host:port

    string portStr;
    string hostStr;
    size_t colonPos = token->find(":");
    if (colonPos != string::npos) { // host:port
        if (colonPos == 0)
            THROW_EXCEPTION_WITH_INFO(ERR_HOST);
        hostStr = token->substr(0, colonPos);
        portStr = token->substr(colonPos + 1);
    }
    else if (token->find(".") != string::npos)
        hostStr = *token;
    else if (is_number(*token))
        portStr = *token;
    else if (token->find_first_of(":") != token->find_last_of(":"))
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

    if (*(token + 1) == "default_server") {
        defaultServer = true;
        ++token; // move to default_server
    }

    check_semicolon(token);

    return port;
}

static void parse_server_name(vector<string>::const_iterator& token, string& serverName)
{
    ++token; // move to server name
    serverName = *token;
    if (is_keyword(serverName))
        THROW_EXCEPTION_WITH_INFO(ERR_SERVER_NAME);
    check_semicolon(token);
}

static void parse_root(vector<string>::const_iterator& token, string& root)
{
    ++token; // move to root path
    root = *token;
    if (root[root.size() - 1] == '/') // remove trailing slash
        root.erase(root.size() - 1);
    check_semicolon(token);
}

static size_t parse_client_max_body_size(vector<string>::const_iterator& token)
{
    ++token; // move to max body size

    string maxBodySize = *token;
    if (maxBodySize.find_first_not_of("0123456789kKmMgG.") != string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_INVALID_CHAR);
    else if (maxBodySize.find_first_of("0123456789") == string::npos)
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_SIZE);
    else if (maxBodySize.find_first_of("kKmMgG") == string::npos) {
        if (maxBodySize == "0") {
            check_semicolon(token);
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

    check_semicolon(token);

    return size * multiplier;
}

static bool parse_autoindex(vector<string>::const_iterator& token)
{
    ++token; // move to on/off
    string autoindex = *token;
    if (autoindex != "on" && autoindex != "off")
        THROW_EXCEPTION_WITH_INFO(ERR_AUTOINDEX);
    check_semicolon(token);
    return autoindex == "on";
}

static vector<string> parse_allow_methods(vector<string>::const_iterator& token)
{
    ++token; // move to methods

    vector<string> methods;
    while (*token != ";") {
        if (*token == "GET" || *token == "POST" || *token == "DELETE")
            methods.push_back(*token);
        else
            THROW_EXCEPTION_WITH_INFO(ERR_METHOD);
        ++token;
    }

    if (methods.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_EMPTY_METHODS);

    --token; // move back to last method
    check_semicolon(token);

    return methods;
}

static void parse_http_redirection(vector<string>::const_iterator& token,
    RedirectionMap&                                                redirMap)
{
    ++token; // move to the URI being redirected
    string redirectFrom = *token;
    if (redirectFrom[0] != '/') // uri should begin with /
        THROW_EXCEPTION_WITH_INFO(ERR_REDIRECT_SLASH);
    else if (redirectFrom.find_first_of("/") != redirectFrom.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_REDIRECT_DUP_SLASH);

    ++token; // move to the target URI
    string redirectTo = *token;
    if (redirectTo[0] != '/') // uri should begin with /
        THROW_EXCEPTION_WITH_INFO(ERR_REDIRECT_SLASH);
    else if (redirectTo.find_first_of("/") != redirectTo.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_REDIRECT_DUP_SLASH);

    redirMap[redirectFrom] = redirectTo;

    check_semicolon(token);
}

/* ----------------------- PARSING CONTEXT FUNCTIONS ------------------------ */

static void parse_location_context(vector<string>::const_iterator& token,
    ServerConfig&                                                  server)
{
    ++token; // move to location uri

    string uri;
    uri = *token;
    if (uri[0] != '/') // uri should begin with /
        THROW_EXCEPTION_WITH_INFO(ERR_URI_MISSING_SLASH);
    else if (uri.find_first_of("/") != uri.find_last_of("/"))
        THROW_EXCEPTION_WITH_INFO(ERR_URI_DUPLICATE_SLASH);

    if (server.locations.find(uri) != server.locations.end())
        THROW_EXCEPTION_WITH_INFO(ERR_DUPLICATE_LOCATION);

    ++token; // move to {

    Location location;
    location.root        = server.root;
    location.indexFile   = server.indexFile;
    location.autoindex   = server.autoindex;
    location.maxBodySize = server.maxBodySize;
    location.methods     = server.methods;

    set<string> parsedDirectives;
    while (*token != "}") {
        if (*token != ";" && *token != "}")
            check_duplicate_element(parsedDirectives, *token, "location");
        if (*token == "root")
            parse_root(token, location.root);
        else if (*token == "client_max_body_size")
            location.maxBodySize = parse_client_max_body_size(token);
        else if (*token == "allow_methods")
            location.methods = parse_allow_methods(token);
        else if (*token == "index")
            parse_index(token, location.indexFile, location.root + uri);
        else if (*token == "autoindex")
            location.autoindex = parse_autoindex(token);
        else if (*token == ";" || *token == "{")
            ++token;
        else
            THROW_EXCEPTION_WITH_INFO(ERR_LOCATION_TOKENS);
    }

    if (parsedDirectives.find("root") == parsedDirectives.end() && location.root.empty())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);
    else if (parsedDirectives.find("allow_methods") == parsedDirectives.end())
        THROW_EXCEPTION_WITH_INFO(ERR_MISSING_METHODS);

    server.locations[uri] = location;

    ++token; // move to next directive
}

static ServerConfig parse_server_context(vector<string>::const_iterator& token)
{
    ++token; // move to {

    set<string>  parsedDirectives;
    ServerConfig server;
    while (*token != "}") {
        if (*token != ";" && *token != "}" && *token != "error_page" &&
            *token != "location")
            check_duplicate_element(parsedDirectives, *token, "server");
        if (*token == "listen")
            server.port = parse_listen(token, server.host, server.defaultServer);
        else if (*token == "root")
            parse_root(token, server.root);
        else if (*token == "client_max_body_size")
            server.maxBodySize = parse_client_max_body_size(token);
        else if (*token == "autoindex")
            server.autoindex = parse_autoindex(token);
        else if (*token == "index")
            parse_index(token, server.indexFile, server.root);
        else if (*token == "server_name")
            parse_server_name(token, server.serverName);
        else if (*token == "allow_methods")
            server.methods = parse_allow_methods(token);
        else if (*token == "error_page")
            parse_error_page(token, server.errorPages, server.root);
        else if (*token == "redirect")
            parse_http_redirection(token, server.redirections);
        else if (*token == "location")
            parse_location_context(token, server);
        else if (*token == ";" || *token == "{")
            ++token;
        else
            THROW_EXCEPTION_WITH_INFO(ERR_SERVER_TOKENS);
    }

    if (server.locations.empty()) {
        if (parsedDirectives.find("root") == parsedDirectives.end())
            THROW_EXCEPTION_WITH_INFO(ERR_MISSING_ROOT);
        else if (parsedDirectives.find("allow_methods") == parsedDirectives.end())
            THROW_EXCEPTION_WITH_INFO(ERR_MISSING_METHODS);
    }

    ++token; // move to next context

    return server;
}

/* ---------------------------- PARSING FUNCTIONS --------------------------- */
vector<ServerConfig> parse_config_file(const string& filePath)
{
    LOG_INFO("Parsing " + filePath);

    string         fileContent = read_file(filePath);
    vector<string> tokens      = tokenize_content(fileContent);
    validate_braces(tokens);

    vector<ServerConfig> servers;
    bool                 foundDefaultServer = false;

    vector<string>::const_iterator token = tokens.begin();
    while (token != tokens.end()) {
        if (*token == "server")
            servers.push_back(parse_server_context(token));
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
