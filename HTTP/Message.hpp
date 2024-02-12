#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "enum_utils.tpp"
#include <vector>

namespace webserv
{
namespace HTTP
{

/* ------------------------------- CRLF string ------------------------------ */
static const std::string CRLF = "\r\n";

/* ------------------------------- METHOD ENUM ------------------------------ */
#define METHOD_ENUMS \
	X(GET) \
	X(HEAD) \
	X(POST) \
	X(PUT) \
	X(DELETE) \
	X(CONNECT) \
	X(OPTIONS) \
	X(TRACE) \
	X(PATCH)

// X macro to define the enum
#define X(a) a,
enum METHOD { METHOD_ENUMS };
#undef X

// defining the char * array
#define X(a) #a, // stringify the enum
template<> const char *enumStrings<METHOD>::data[] = {METHOD_ENUMS};
#undef X

}; // namespace HTTP
}; // namespace webserv

#endif // MESSAGE_HPP
