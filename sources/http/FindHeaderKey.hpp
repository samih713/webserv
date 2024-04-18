#include "webserv.hpp"

#ifndef FINDHEADERKEY_HPP
#define FINDHEADERKEY_HPP

struct FindHeaderKey {
    FindHeaderKey(const string& key) : key(key) {};
    bool operator()(const pair<string, string>& header) { return header.first == key; }

private:
    const string& key;
};

#endif // FINDHEADERKEY_HPP
