#include "Header.hpp"
#include "Message.hpp"
#include "debug.hpp"
#include "webserv.hpp"

// helper utils
static void   check_line_terminator(istream& message, const string& check);
static bool   peek_line_terminator(istream& message, const string& check);
static void   replace_spaces(string& resource);
static void   to_lower(string& str);
static string get_field_value(istringstream& message);


void Header::parse_header(istringstream& message)
{
    // Request line
    parse_request_line(message);
    // Headers Fields
    while (true && !message.eof() && !peek_line_terminator(message, CRLF))
        add_header(message);
    // end of header fields
    check_line_terminator(message, CRLF);
    // after all headers are completed
    process_content();
}

void Header::parse_request_line(istringstream& message)
{
    // accepted version
    static const string acceptedVersion("HTTP/1.1");

    char singleSpace[2] = { 0 };

    std::noskipws(message);
    message >> method >> singleSpace[0] >> resource >> singleSpace[1] >> version;
    if (strncmp(singleSpace, "  ", 2))
        message.setstate(std::ios::failbit);
    if (strncmp(acceptedVersion.c_str(), version.c_str(),
            std::max(version.length(), acceptedVersion.length())))
        message.setstate(std::ios::failbit);
    check_line_terminator(message, CRLF);
    replace_spaces(resource);
}

void Header::add_header(istringstream& message)
{
    static string fieldName;
    static string fieldValue;

    fieldName.clear();
    fieldValue.clear();

    std::getline(message, fieldName, ':');
    if (fieldName.find(' ') != string::npos)
        message.setstate(std::ios::failbit);
    to_lower(fieldName);
    fieldValue = get_field_value(message);

    // set-cookie is a special case where it would be on different fields
    if (fields.find(fieldName) != fields.end())
        fields.at(fieldName).append(", " + fieldValue);
    else
        fields.insert(std::make_pair(fieldName, fieldValue));
}

void Header::process_content()
{
    // body size
    const HeaderMap::iterator encoding      = fields.find("transfer-encoding");
    const HeaderMap::iterator contentLength = fields.find("content-length");

    // if content-length && chunked is last ... etc do that whole logic here
    if (encoding != fields.end())
        is_chunked(encoding);
    else if (contentLength != fields.end())
        find_content_length(contentLength);
}

void Header::is_chunked(const __attribute__((unused)) HeaderMap::const_iterator it)
{
    // const string& encoding(it->second);

    // split up int a encodings
    // string::const_iterator begin = encoding.begin();
    // string::const_iterator end   = encoding.end();

    // if chunked is the last encoding, set process chunked
    // if not set status to 400
    DEBUGASSERT(false && "not yet implemented");
}

void Header::find_content_length(const HeaderMap::const_iterator it)
{
    if (it == fields.end())
        bodySize = NOT_SPECIFIED;

    istringstream length;

    length.exceptions(std::ios::failbit | std::ios::badbit);
    length >> bodySize;

    if (!length.eof())
        length.setstate(std::ios::failbit);
}

// helper utils
static string get_field_value(istringstream& message)
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

static void to_lower(string& str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::tolower(str[i]);
}

static void check_line_terminator(istream& message, const string& check)
{
    string line_terminator;
    std::noskipws(message);

    string::const_iterator check_end = check.end();
    for (string::const_iterator it = check.begin(); it != check_end && message.good();
         it++)
        line_terminator += message.get();

    if (line_terminator != check)
        message.setstate(std::ios::failbit);

    std::skipws(message);
}

static bool peek_line_terminator(istream& message, const string& check)
{
    string         line_terminator;
    std::streampos initialPos = message.tellg();
    std::noskipws(message);

    string::const_iterator check_end = check.end();
    for (string::const_iterator it = check.begin(); it != check_end && message.good();
         it++)
        line_terminator += message.get();

    message.clear();
    message.seekg(initialPos, std::ios::beg);

    if (line_terminator != check)
        return false;

    std::skipws(message);
    return true;
}

// replace %20 with space
// TODO replace all other special characters
static void replace_spaces(string& resource)
{
    size_t pos;
    while ((pos = resource.find("%20")) != std::string::npos)
        resource.replace(pos, 3, " ");
}
