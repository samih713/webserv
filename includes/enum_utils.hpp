#include <iostream>
#include <vector>

#ifndef ENUM_UTILS_HPP
#define ENUM_UTILS_HPP

namespace webserv
{

template<typename T>
struct enumStrings
{
        static const char *data[];
};

template<typename T>
struct enumRefHolder
{
        T &enum_;
        enumRefHolder(T &enum__)
            : enum_(enum__)
        {}
};

template<typename T>
struct enumConstRefHolder
{
        const T &enum_;
        enumConstRefHolder(const T &enum__)
            : enum_(enum__)
        {}
};

typedef std::vector<std::string>::const_iterator enumStringsIterator;

template<typename T>
std::istream &operator>>(std::istream &str, enumRefHolder<T> const &data)
{

    std::string value;
    str >> value;

    char **begin = (char **)enumStrings<T>::data;
    char **end = begin + sizeof(enumStrings<T>::data) / sizeof(enumStrings<T>::data[0]);
    for (char **it = begin; it != end; ++it)
    {
        if (*it == value)
        {
            data.enum_ = static_cast<T>(std::distance(begin, it));
            return str;
        }
    }
    str.setstate(std::ios_base::failbit);
    return str;
}


template<typename T>
std::ostream &operator<<(std::ostream &str, const enumConstRefHolder<T> &data)
{
    // TODO: Fix object instantiation error for Mac Compilation
    // return str << enumStrings<T>::data[static_cast<size_t>(data.enum_)];
    (void) data;
    return str << "GET";
}

template<typename T>
enumConstRefHolder<T> enumToString(const T &enum__)
{
    return enumConstRefHolder<T>(enum__);
}

template<typename T>
enumRefHolder<T> enumFromString(T &enum__)
{
    return enumRefHolder<T>(enum__);
}
}; // namespace webserv

#endif // ENUM_UTILS_HPP
