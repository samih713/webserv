#include "../request/Request.hpp"
#include "../response/Response.hpp"
#include "../includes/webserv.hpp"

int main()
{
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
	METHOD m = GET;
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
