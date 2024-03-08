#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "Request.hpp"
#include "RequestHandlerFactory.hpp"
#include "Response.hpp"
#include "webserv.hpp"
#include <iostream>
#include <stdexcept>
#include <unistd.h>

using namespace webserv::http;

int main()
{
#if 1 // response test
    try
    {
        Request request(sample_request);
        cout << request;
        IRequestHandler *handler =
            RequestHandlerFactory::MakeRequestHandler(request.get_method());
        Response response = handler->handle_request(request);
        response.write_response(STDOUT_FILENO);
        delete handler;
    }
    catch (std::runtime_error &e)
    {
        cerr << e.what() << std::endl;
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
