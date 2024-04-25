#include "Header.hpp"

Header::Header() : bodySize(NOT_SPECIFIED), state(NOT_READY), chunked(false) {}

Header::Header(const Header& other)
    : method(other.method), resource(other.resource), version(other.version),
      cgiResource(other.cgiResource), bodySize(other.bodySize), fields(other.fields), state(other.state),
      chunked(other.chunked)
{}

Header::~Header() {};

Header& Header::operator=(const Header&)
{
    return *this;
};

bool Header::operator()(HEADER_STATE s)
{
    return state == s;
}
