#include "IRequestHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <iostream>
#include <stdexcept>

// TODO
// [ ] finish up the resource handling for get-requests
// [ ] Implement logging
// [ ] Implement unit-testing (with a testing framework)
// [ ] strict space parsing (only 1 space)
// [ ] solve linkage issue (with data)
// [x] Implement response
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
        Request          request(sample_request);
        IRequestHandler *handler =
            RequestHandlerFactory::MakeRequestHandler(request.get_method());
        Response response = handler->handle_request(request);
        response.write_response(cout);
        delete handler;
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
