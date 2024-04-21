#include "Message.hpp"
#include "Request.hpp"

// helper functions
static void   check_line_terminator(istream& is, const string& check);
static void   replace_spaces(string& resource);
static bool   peek_line_terminator(istream& is, const string& check);
static string find_value(istringstream& message);
static void   add_header(istringstream& message, HeaderMap& headerFields);
static void   to_lower(string& str);

// throws ios failure
bool Request::parse_request(const ServerConfig& config)
{
    message.exceptions(std::ios::failbit | std::ios::badbit);
    if (headerState == READY_TO_PARSE)
        parse_header();
    if (headerState == PARSED) {
        if (expectedBodySize == NOT_SPECIFIED)
            parsed = true;
        else
            parse_body();
    }
    apply_config(config);
    return parsed;
}

void Request::parse_request_line()
{
    char singleSpace[2] = { 0 };
    std::noskipws(message);
    message >> method >> singleSpace[0] >> resource >> singleSpace[1] >> httpVersion;
    if (strncmp(singleSpace, "  ", 2))
        message.setstate(std::ios::failbit);
    check_line_terminator(message, CRLF);
    replace_spaces(resource);
}

void Request::parse_header_fields_content()
{
    parse_content_length(headerFields.find("content-length"));
}

void Request::parse_header()
{
    parse_request_line();

    // Headers Fields
    while (true && !message.eof() && !peek_line_terminator(message, CRLF))
        add_header(message, headerFields);

    // end of header fields
    check_line_terminator(message, CRLF);

    // set state to parsed
    headerState = PARSED;
}

void static add_header(istringstream& message, HeaderMap& headerFields)
{
    static string fieldName;
    static string fieldValue;

    fieldName.clear();
    fieldValue.clear();

    std::getline(message, fieldName, ':');
    if (fieldName.find(' ') != string::npos)
        message.setstate(std::ios::failbit);
    to_lower(fieldName);
    fieldValue = find_value(message);
    // set-cookie is a special case
    if (headerFields.find(fieldName) != headerFields.end())
        headerFields[fieldName].append(" , " + fieldValue);
    else
        headerFields.insert(std::make_pair(fieldName, fieldValue));
}

// applies the configuration parameters to the request
void Request::apply_config(const ServerConfig& config)
{
    resource    = config.serverRoot + resource;
    cgiResource = config.serverRoot + resource; // change to cgi root
    maxBodySize = config.maxBodySize;
}

// TODO handle chunked encoding
void Request::parse_body()
{
    if (expectedBodySize != NOT_SET)
        body.append(message.str().substr(message.tellg()),
            (expectedBodySize <= maxBodySize ? expectedBodySize : 0));
    parsed = true;
}

static void check_line_terminator(istream& is, const string& check)
{
    string line_terminator;
    std::noskipws(is);

    string::const_iterator check_end = check.end();
    for (string::const_iterator it = check.begin(); it != check_end && is.good(); it++)
        line_terminator += is.get();

    if (line_terminator != check)
        is.setstate(std::ios::failbit);

    std::skipws(is);
}

static bool peek_line_terminator(istream& is, const string& check)
{
    string         line_terminator;
    std::streampos initialPos = is.tellg();
    std::noskipws(is);

    string::const_iterator check_end = check.end();
    for (string::const_iterator it = check.begin(); it != check_end && is.good(); it++)
        line_terminator += is.get();

    is.clear();
    is.seekg(initialPos, std::ios::beg);

    if (line_terminator != check)
        return false;

    std::skipws(is);
    return true;
}

static string find_value(istringstream& message)
{
    string fieldValue;
    std::getline(message, fieldValue);
    string::size_type begin = fieldValue.find_first_not_of(" ");
    string::size_type end   = fieldValue.find_last_not_of(CRLF);
    if (begin != std::string::npos && end != std::string::npos)
        fieldValue = fieldValue.substr(begin, end - begin + 1);
    else if (begin != std::string::npos)
        fieldValue = fieldValue.substr(begin);
    return fieldValue;
}

void Request::parse_content_length(const HeaderMap::const_iterator it)
{
    if (it == headerFields.end()) {
        expectedBodySize = NOT_SPECIFIED;
        return;
    }

    istringstream length;

    length.exceptions(std::ios::failbit | std::ios::badbit);
    length >> expectedBodySize;

    if (!length.eof())
        length.setstate(std::ios::failbit);
}

// replace %20 with space
// TODO replace all other special characters
static void replace_spaces(string& resource)
{
    size_t pos;
    while ((pos = resource.find("%20")) != std::string::npos)
        resource.replace(pos, 3, " ");
}

static void to_lower(string& str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::tolower(str[i]);
}
