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

const vector<char> GetRequestHandler::get_resource(const string &resource,
                                                   const vsp    &headers)
{
    (void)headers;
    string       temp = "";
    string       resource_type = "";
    vector<char> body;

    // try to locate the resource
    resource_type = find_resource_type(resource);
    if (resource_type.length() != 0)
        add_header(std::make_pair<string, string>("Content-Type", resource_type));

    /* need to build resource location from the
     * root directory in server configuration
     * */

    ifstream resource_file(resource.c_str(), std::ios_base::binary);
    // if not found set status to not found and return
    if (resource_file.fail())
    {
        status = NOT_FOUND;
        // set the body to page not found
        // page not found needs to optimized opened once in case multiple calls???
        // ifstream page_not_found(resource.c_str(), std::ios_base::binary);
        // body = vector<char>((std::istreambuf_iterator<char>(page_not_found)),
        //                     std::istreambuf_iterator<char>());
        return body;
    }

    /* need to authenticate/authorize
     * authentication function goes here for the requested resource
     * */

    /* not a directory or how are directories handled
     * */

    /* determine type for content-type in header_field
     * */

    /* determine content length
     * */

    /* caching control
     * */

    /* compression/encoding
     * */

    /* support range requests, usefull for large files
     * */

    body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
                        std::istreambuf_iterator<char>());
    return body;
}


Response GetRequestHandler::handle_request(const Request &request)
{
    DEBUG_MSG("Handling get request ... ", B);

    vsp request_headers = request.get_headers();
    try
    {
        body = get_resource(request.get_resource(), request_headers);
        // if no issues set status
        status = OK;
    }
    catch (std::runtime_error &re)
    {
        DEBUG_MSG(re.what(), R);
        status = NOT_FOUND;
        // empty out the body if anything
        body.clear();
    }
    return Response(status, response_headers, body);
}

// utils

void IRequestHandler::add_header(const pair<string, string> header_field)
{
    response_headers.push_back(header_field);
}
