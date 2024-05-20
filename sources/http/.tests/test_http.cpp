#include "Request.hpp"
#include "Response.hpp"
#include "webserv.hpp"

int main()
{
#if 0  // parsing test
    try
    {
        Request request(sample_request);
        cout << request;
    }
    catch (runtime_error &e)
    {
        cerr << e.what() << endl;
    }
#endif // parsing test

#if 0  // basic test
	string m = GET;
	string path;
	string version;

	stringstream line(sample_message);
	line.exceptions(std::ios::failbit | std::ios::badbit);

	cout << "GET: " << enumToString(m) << endl;

	line >> enumFromString(m) >> path >> version;
	if (line.fail())
		cout << "Failed to parse" << endl;
	else {
		cout << "METHOD is : " << m << endl;
		cout << "PATH is : " << path << endl;
		cout << "VERSION is : " << version << endl;
	}
#endif // basic test
    return 0;
}
