#include "../../includes/webserv.hpp"

#ifndef FINDHEADERKEY_HPP
#define FINDHEADERKEY_HPP


namespace webserv
{
namespace http
{

struct FindHeaderKey
{
        FindHeaderKey(const string &key)
            : key(key){};
        bool operator()(const pair<string, string> &header)
        {
            return header.first == key;
        }

    private:
        const string &key;
};
}; // http
}; // webserv

#endif // FINDHEADERKEY_HPP
