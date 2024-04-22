#include "Message.hpp"
#include "Request.hpp"
#include "enum_utils.hpp"

// defining the METHOD char * array
#define X(a) ws_tostr(a), // stringify the enum
template<>
const char* enumStrings<METHOD>::data[] = { METHOD_ENUMS };
#undef X

// helper functions
static void   check_line_terminator(istream& is, const string& check);
static void   replace_spaces(string& resource);
static bool   peek_line_terminator(istream& is, const string& check);
static string find_value(stringstream& message);

// throws ios failure
bool Request::parse_request(const ServerConfig &config)
{
    message.exceptions(std::ios::failbit | std::ios::badbit);
    if (headerReady && expectedBodySize == NOT_SET) {
        parse_header();
        apply_config(config);
        if (expectedBodySize != NOT_SPECIFIED)
            parse_body();
    }
    return parsed;
}


void Request::parse_content_length(const string& contentLength)
{
    stringstream length(contentLength);
    length.exceptions(std::ios::failbit | std::ios::badbit);
    length >> expectedBodySize;
    if (!length.eof())
        length.setstate(std::ios::failbit);
}

// TODO headers to parse multiple line field-values, and multi-line
void Request::parse_header()
{
    string fieldName;
    string fieldValue;

    // Request Line
    message >> enumFromString(method) >> resource >> http_version;
    check_line_terminator(message, CRLF);
    // replace %20 with space
    replace_spaces(resource);
    // Headers
    while (true && !message.eof()) {
        if (peek_line_terminator(message, CRLF))
            break;
        std::getline(message, fieldName, ':');
        if (fieldName.find(' ') != string::npos)
            message.setstate(std::ios::failbit);
        fieldValue = find_value(message);
        header_fields.push_back(make_pair(fieldName, fieldValue));
        if (fieldName == "Content-length")
            parse_content_length(fieldValue);
        else
            expectedBodySize = NOT_SPECIFIED;
    }
    if (expectedBodySize == NOT_SPECIFIED)
        parsed = true;
}

// applies the configuration parameters to the request
void Request::apply_config(const ServerConfig& config)
{
    resource = config.serverRoot + resource;
    cgiResource = config.serverRoot + resource; // change to cgi root
}

// TODO handle chunked encoding
void Request::parse_body()
{
    if (!message.eof())
        std::getline(message, body, '\0');
    if (expectedBodySize != NOT_SET && expectedBodySize != NOT_SPECIFIED)
        body = body.substr(0, expectedBodySize);
    parsed = true;
}

/**
 * @brief Checks if the input stream has the specified line terminator.
 *
 * This function reads characters from the input stream until it reaches the end of the
 * specified line terminator string or the end of the stream. If the characters read do
 * not match the line terminator string, the function sets the failbit of the input
 * stream.
 *
 * @param is The input stream to check for the line terminator.
 * @param check The line terminator string to compare against the characters read from the
 * input stream.
 *
 * @return void
 *
 * @throws None
 */
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

/**
 * Check if the next characters in the input stream match a given string.
 *
 * This function reads characters from the input stream `is` and compares them to the
 * characters in the `check` string. If the characters in the input stream match the
 * characters in the `check` string, the function returns true. If the characters do
 * not match, the function returns false.
 *
 * @param is The input stream to read characters from.
 * @param check The string to compare the characters read from the input stream to.
 *
 * @return true if the characters in the input stream match the characters in the
 * `check` string, false otherwise.
 *
 * @throws None
 */
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

/**
 * @brief Extracts a value from a message, leading and trailing whitespace characters are
 * removed before returning it.
 *
 * @param message The stringstream from which to extract the value.
 * @return The extracted value as a string.
 */
static string find_value(stringstream& message)
{
    string fieldValue;
    std::getline(message, fieldValue);
    string::size_type begin = fieldValue.find_first_not_of(" ");
    string::size_type end   = fieldValue.find_last_not_of(CRLF);
    if (begin != string::npos && end != string::npos)
        fieldValue = fieldValue.substr(begin, end - begin + 1);
    else if (begin != string::npos)
        fieldValue = fieldValue.substr(begin);
    return fieldValue;
}

/*
 * @brief Replaces all occurrences of "%20" in the resource string with a space character.
 *
 * @param resource The resource string to replace "%20" with a space character.
 */
static void replace_spaces(string& resource)
{
    size_t pos;
    while ((pos = resource.find("%20")) != string::npos)
        resource.replace(pos, 3, " ");
}
