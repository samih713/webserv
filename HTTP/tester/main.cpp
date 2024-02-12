#include "../Request.hpp"
#include <iostream>
#include <sstream>

// TODO
// [ ] solve linkage issue
// [x] set the stream to throw exception on fail
// [x] sometimes segfaults when parsing, keep repeating to reproduce

using namespace webserv;

// sample message for testing
static std::string sample_message = "PUT / HTTP/1.1\r\n"
	"Host: localhost:8080\r\n"
	"User-Agent: curl/7.68.0\r\n"
	"Accept: */*\r\n"
	"\r\n";

int main()
{
#if 0 // parsing test
	HTTP::Request r(sample_message);
	r.parse();
#endif // parsing test

#if 1 // basic test
	HTTP::METHOD m;
	// HTTP::METHOD m = HTTP::GET;
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
