#include "webserv.hpp"

#ifndef REQUEST_UTILS_HPP
#define REQUEST_UTILS_HPP

inline void to_lower(string& str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::tolower(str[i]);
}

/**
 * Checks and consumes "check" if the next characters in the input stream match. If the
 * characters do not match, the input stream is marked as failed.
 *
 * @param message The input stream to check.
 * @param check The string to compare against the characters in the input stream.
 */
inline void validate_terminator(stringstream& message, const string& check)
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

/**
 * Checks if the given string `check` is present in the remaining part of the `message`
 * stream.
 *
 * @param message The input stringstream containing the message.
 * @param check The string to search for in the message.
 * @return `true` if the `check` string is found in the remaining part of the `message`
 * stream, `false` otherwise.
 */
inline bool find_terminator(stringstream& message, const string& check)
{
    return message.str().find(check, message.tellg()) != string::npos;
}

/**
 * Checks if the next character[s] in the given stringstream matches the specified string.
 *
 * @param message The stringstream to read from.
 * @param check The string to compare against the characters in the stringstream.
 * @return True if the next characters matches the specified string, false otherwise.
 */
inline bool peek_terminator(stringstream& message, const string& check)
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

#endif // REQUEST_UTILS_HPP
