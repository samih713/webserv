#include "Request.hpp"
#include "ResponseBuilder.hpp"
// #include "Response.hpp"
#include <iostream>
#include <stdexcept>

// TODO
// [ ] Implement response
// [ ] Implement logging
// [ ] Implement unit-testing (with a testing framework)
// [ ] solve linkage issue (with data)
// [ ] strict space parsing (only 1 space)
// [x] set the stream to throw exception on fail
// [x] sometimes segfaults when parsing, keep repeating to reproduce

using namespace webserv::http;

// sample http request

// check resource ? do i have
// check does user have permission
// headers i want?


int main()
{
#if 1 // response test
    try
    {
        Request         request(sample_request);
        ResponseBuilder builder(request);
        builder.get_response();
        // std::cout << endl << endl << endl << request;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
#endif // response test

#if 0  // parsing test
    try
    {
        Request request(sample_request);
        std::cout << request;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
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
