#include "IRequestHandler.hpp"
#include "Message.hpp"
#include "debug.hpp"
#include "webserv.hpp"
#include <ios>
#include <stdexcept>

using namespace webserv::http;

struct FindKey
{
        FindKey(const string &key)
            : key(key){};
        bool operator()(const pair<string, string> &header)
        {
            return header.first == key;
        }

    private:
        const string &key;
};


const vector<char> GetRequestHandler::get_resource(const string &resource,
                                                   const vsp    &headers)
{
    (void)headers;
    string       temp = "";
    vector<char> body;

    // try to locate the resource

    // parse accepted formats

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
    return Response(status, request_headers, body);
}
