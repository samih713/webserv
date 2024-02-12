#include "Request.hpp"
#include <sstream>
#include <cctype>

// sample http request
//
// GET /docs/tutorials/linux/shellscripts/howto.html HTTP/1.1
// Host: Linode.com
// User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.8) Gecko/20091102
// Firefox/3.5.5 Accept: text/html,application/xhtml+xml, Accept-Language: en-us
// Accept-Encoding: gzip,deflate
// Accept-Charset: ISO-8859-1,utf-8
// Cache-Control: no-cache

// constructor
using namespace webserv::HTTP;

Request::Request(const std::string &rawRequest)
    : rawRequest(rawRequest)
{}

// destructor
Request::~Request()
{}

// copy
Request::Request(const Request &other)
    : rawRequest(other.rawRequest)
    , method(other.method)
{}

// member functions
const std::string &Request::get_raw_request() const
{
    return rawRequest;
}

void Request::parse()
{
	std::stringstream message(rawRequest);
	message >> enumFromString(method) >> path >> version;
	if (message.fail()) {
		throw std::runtime_error("Invalid HTTP method");
	}
}
