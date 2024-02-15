#include "../request/Request.hpp"
#include <iostream>
#include <sstream>

// TODO
// [ ] URI syntax
// [ ] solve linkage issue
// [ ] strict space parsing (only 1 space)
// [ ] add a custom exception class and throw that exception with a messag
// [x] set the stream to throw exception on fail
// [x] sometimes segfaults when parsing, keep repeating to reproduce

using namespace webserv;

// sample http request
//
static const std::string sample_message =
    "GET /docs/tutorials/linux/shellscripts/howto.html HTTP/1.1\r\n"
    "Host: Linode.com\r\n"
    "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.8) "
    "Gecko/20091102 Firefox/3.5.5 \r\n"
    "Accept: text/html,application/xhtml+xml, Accept-Language: en-us\r\n"
    "Accept-Encoding: gzip,deflate\r\n"
    "Accept-Charset: ISO-8859-1,utf-8\r\n"
    "Cache-Control: no-cache\r\n"
    "\r\n";


int main()
{
#if 1 // parsing test
    http::Request r(sample_message);
    try
    {
        r.parse();
        std::cout << r;
    }
    catch (std::ios_base::failure &failure)
    {
        std::cerr << "Invalid http request" << std::endl;
    }
#endif // parsing test

#if 0  // basic test
	HTTP::METHOD m = HTTP::GET;
	std::string path;
	std::string version;

	std::stringstream line(sample_message);
	line.exceptions(std::ios::failbit | std::ios::badbit);

	std::cout << "GET: " << enumToString(m) << std::endl;

	line >> enumFromString(m) >> path >> version;
	if (line.fail())
		std::cout << "Failed to parse" << std::endl;
	else {
		std::cout << "METHOD is : " << enumToString(m) << std::endl;
		std::cout << "PATH is : " << path << std::endl;
		std::cout << "VERSION is : " << version << std::endl;
	}
#endif // basic test
    return 0;
}
