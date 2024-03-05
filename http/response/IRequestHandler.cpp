#include "IRequestHandler.hpp"
#include "FileType.hpp"
#include "Message.hpp"
#include "debug.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <cstring>
#include <ios>
#include <stdexcept>
#include <utility>

using namespace webserv::http;

GetRequestHandler::GetRequestHandler()
{
    DEBUG_MSG("GetRequestHandler constructor called", B);

	DEBUGASSERT("Find a way to only load 404 page once" && false);

    _not_found.open("../resources/sample_pages/404.html", std::ios_base::binary);
    if (_not_found.fail())
    {
        DEBUG_MSG("My guy", B);
    }
    not_found = vector<char>((std::istreambuf_iterator<char>(_not_found)),
                             std::istreambuf_iterator<char>());
}

GetRequestHandler::~GetRequestHandler()
{
    DEBUG_MSG("GetRequestHandler destructor called", B);
};

// parse accepted formats
inline const string find_resource_type(const string &resource)
{
    size_t extension_index = resource.find_last_of('.');
    string file_extension;
    if (extension_index != string::npos)
        file_extension = resource.substr(extension_index + 1);
    else
        file_extension = "";
    return (fileTypes.find(file_extension)->second);
}


// when does this close, should be done in the server

const vector<char> GetRequestHandler::get_resource(const string &resource,
                                                   const vsp    &headers)
{
    (void)headers;
    string       temp = "";
    string       resource_type = "";
    vector<char> body;


    /* need to build resource location from the
     * root directory in server configuration
     * */
    ifstream resource_file(resource.c_str(), std::ios_base::binary);
    if (resource == "/") {
		DEBUGASSERT("Solve directory issue" && false);
        resource_file.setstate(std::ios_base::failbit);
	}
    if (resource_file.fail())
    {
        status = NOT_FOUND;
        return not_found;
    }

    /* need to authenticate/authorize
     * authentication function goes here for the requested resource
     * */

    /* not a directory or how are directories handled */

    /* determine type for content-type in header_field */
    resource_type = find_resource_type(resource);
    if (resource_type.length() != 0)
        add_header(std::make_pair<string, string>("Content-Type", resource_type));
    // default header server name
    add_header(std::make_pair<string, string>("Server", "The Wired"));
    // order of the headers

    /* determine content length */
    resource_file.seekg(0, std::ios_base::end);
    size_t resource_size = resource_file.tellg();
    add_header(std::make_pair<string, string>("Content-length", ws_itoa(resource_size)));
    resource_file.seekg(0, std::ios_base::beg);

    /* caching control */

    /* compression/encoding
     * */

    /* support range requests, usefull for large files
     * */

    body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
                        std::istreambuf_iterator<char>());
    status = OK;
    return body;
}


Response GetRequestHandler::handle_request(const Request &request)
{
    DEBUG_MSG("Handling get request ... ", B);

    vsp request_headers = request.get_headers();
    try
    {
        body = get_resource(request.get_resource(), request_headers);
    }
    catch (std::runtime_error &re)
    {
        DEBUG_MSG(re.what(), R);
        status = NOT_FOUND;
        // empty out the body if anything
        // body.clear();
    }
    return Response(status, response_headers, body);
}

// utils
