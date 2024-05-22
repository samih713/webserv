#include "PostRequestHandler.hpp"

PostRequestHandler::PostRequestHandler()
{
    DEBUG_MSG("PostRequestHandler constructor called", B);
}

PostRequestHandler::~PostRequestHandler()
{
    DEBUG_MSG("PostRequestHandler destructor called", B);
}

/**
 * pretty much if a post request is made to a resource that does not exist
 * we create it
 *
 * if the resource is a directory we create an index.html file in it
 * if the resource is a file we append to it
 * 
 * Example of POST request:
 * POST /cgi-bin/process.cgi HTTP/1.1
 * POST /path/to/resource HTTP/1.1
*/
void PostRequestHandler::create_resource(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    const string& resource = request.get_resource();

    /**
     * POST resource can be:
     * - a file
     * - a directory
     * - a cgi script
     * - a non existent resource
     * 
     * 
    */
   (void) cachedPages;
    (void) config;
    (void) resource;
    
}

Response PostRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling POST", W);
    (void) cachedPages;
    (void) config;
    (void) request;

    vector<char> body('a');
    Response     response(NOT_FOUND, response_headers, body);
    return response;
}
