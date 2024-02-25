#include "Message.hpp"
#include "Request.hpp"
#include "debug.hpp"
#include "enum_utils.tpp"
#include <ios>
#include <iostream>
#include <sstream>
#include <utility>

using namespace webserv::http;

// defining the char * array
#define X(a) #a, // stringify the enum
template<>
const char *webserv::enumStrings<METHOD>::data[] = { METHOD_ENUMS };
#undef X

static inline void check_line_terminator(istream &is, const string &check)
{
    std::string line_terminator;
    std::noskipws(is);
    line_terminator += is.get();
    line_terminator += is.get();
    if (line_terminator != check)
    {
        is.setstate(std::ios::failbit);
    }
    std::skipws(is);
}

static inline bool peek_line_terminator(istream &is, const string &check)
{
    std::string line_terminator;
    std::noskipws(is);
    line_terminator += is.get();
    line_terminator += is.get();
    is.putback(line_terminator[1]);
    is.putback(line_terminator[0]);
    if (line_terminator != check)
    {
        return false;
    }
    std::skipws(is);
    return true;
}


// throws ios_base::failure
namespace webserv
{
namespace http
{

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

void Request::parse()
{
    stringstream message(rawRequest);
    string       fieldName;
    string       fieldValue;

    static map<string, int>::const_iterator fieldNameListEnd = fieldNameList.end();
    message.exceptions(std::ios::failbit | std::ios::badbit); // if stream fails throw
    // Request Line
    message >> enumFromString(method) >> resource >> http_version;
    check_line_terminator(message, CRLF);
    // Headers
    while (true)
    {
        std::getline(message, fieldName, ':');
        if (fieldName.find(' ') != string::npos ||
            fieldNameList.find(fieldName) == fieldNameListEnd)
        {
            message.setstate(std::ios::failbit);
        }
        fieldValue = find_value(message);
        header_fields.push_back(std::make_pair(fieldName, fieldValue));
        if (peek_line_terminator(message, CRLF))
            break;
    }
    // Body
    std::getline(message, body, '\0');
}
}; // http
}; // webserv
