#include "Request.hpp"
#include "Response.hpp"
#include "webserv.hpp"

int main()
{
#if 0  // parsing test
    try
    {
        Request request(sample_request);
        std::cout << request;
    }
    catch (runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
#endif // parsing test

#if 0  // basic test
	string m = GET;
	string path;
	string version;

	std::stringstream line(sample_message);
	line.exceptions(std::ios::failbit | std::ios::badbit);

	std::cout << "GET: " << enumToString(m) << std::endl;

	line >> enumFromString(m) >> path >> version;
	if (line.fail())
		std::cout << "Failed to parse" << std::endl;
	else {
		std::cout << "METHOD is : " << m << std::endl;
		std::cout << "PATH is : " << path << std::endl;
		std::cout << "VERSION is : " << version << std::endl;
	}
#endif // basic test
    return 0;
}
