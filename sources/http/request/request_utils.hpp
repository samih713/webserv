#include "webserv.hpp"

#ifndef REQUEST_UTILS_HPP
#define REQUEST_UTILS_HPP

inline void to_lower(string& str)
{
    for (size_t i = 0; i < str.length(); i++)
        str[i] = std::tolower(str[i]);
}

inline void check_line_terminator(stringstream& message, const string& check)
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

inline bool peek_line_terminator(stringstream& message, const string& check)
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
