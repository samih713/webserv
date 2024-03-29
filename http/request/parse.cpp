#include "Message.hpp"
#include "Request.hpp"
#include "enum_utils.hpp"

using namespace webserv::http;

// defining the char * array
#define X(a) ws_tostr(a), // stringify the enum
template<>
const char *webserv::enumStrings<METHOD>::data[] = { METHOD_ENUMS };
#undef X

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
static inline void check_line_terminator(istream &is, const string &check)
{
    string line_terminator;
    std::noskipws(is);

    string::const_iterator check_end = check.end();
    for (string::const_iterator it = check.begin(); it != check_end && is.good(); it++)
    {
        line_terminator += is.get();
    }

    if (line_terminator != check)
    {
        is.setstate(std::ios::failbit);
    }

    std::skipws(is);
}


/**
 * Check if the next characters in the input stream match a given string.
 *
 * This function reads characters from the input stream `is` and compares them to the
 * characters in the `check` string. If the characters in the input stream match the
 * characters in the `check` string, the function returns true. If the characters do not
 * match, the function returns false.
 *
 * @param is The input stream to read characters from.
 * @param check The string to compare the characters read from the input stream to.
 *
 * @return true if the characters in the input stream match the characters in the `check`
 * string, false otherwise.
 *
 * @throws None
 */
static inline bool peek_line_terminator(istream &is, const string &check)
{
    string         line_terminator;
    std::streampos initialPos = is.tellg();
    std::noskipws(is);

    string::const_iterator check_end = check.end();
    for (string::const_iterator it = check.begin(); it != check_end && is.good(); it++)
    {
        line_terminator += is.get();
    }

    is.clear();
    is.seekg(initialPos, std::ios::beg);

    if (line_terminator != check)
    {
        return false;
    }

    std::skipws(is);
    return true;
}


namespace webserv
{
namespace http
{

/**
 * @brief Extracts a value from a message, leading and trailing whitespace characters are
 * removed before returning it.
 *
 * @param message The stringstream from which to extract the value.
 * @return The extracted value as a string.
 */
inline static string find_value(stringstream &message)
{
    string fieldValue;
    std::getline(message, fieldValue);
    string::size_type begin = fieldValue.find_first_not_of(" ");
    string::size_type end = fieldValue.find_last_not_of(CRLF);
    if (begin != std::string::npos && end != std::string::npos)
        fieldValue = fieldValue.substr(begin, end - begin + 1);
    else if (begin != std::string::npos)
        fieldValue = fieldValue.substr(begin);
    return fieldValue;
}


/**
 * @brief Parses the raw request data into method, resource, HTTP version, headers, and
 * body.
 *
 * @throws std::ios_base::failure if there is a failure in parsing the raw request data
 */
void Request::parse()
{
    stringstream message(rawRequest);
    string       fieldName;
    string       fieldValue;

    message.exceptions(std::ios::failbit | std::ios::badbit);

    // Request Line
    message >> enumFromString(method) >> resource >> http_version;
    check_line_terminator(message, CRLF);

    // Headers
    static map<string, int>::const_iterator fieldNameListEnd = fieldNameList.end();
    while (true)
    {
        std::getline(message, fieldName, ':');
        if (fieldName.find(' ') != string::npos ||
            fieldNameList.find(fieldName) == fieldNameListEnd)
        {
            // DEBUGASSERT("The header check is wrong, brave sends some weird headers or
            // maybe all browsers do idk" && false);
        }
        fieldValue = find_value(message);
        header_fields.push_back(std::make_pair(fieldName, fieldValue));
        if (peek_line_terminator(message, CRLF))
            break;
    }

    // Body till the end of the message
    std::getline(message, body, '\0');
}

}; // http
}; // webserv
