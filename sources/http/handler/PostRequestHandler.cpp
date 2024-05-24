#include "PostRequestHandler.hpp"

PostRequestHandler::PostRequestHandler()
{
    DEBUG_MSG("PostRequestHandler constructor called", B);
}

PostRequestHandler::~PostRequestHandler()
{
    DEBUG_MSG("PostRequestHandler destructor called", B);
}

Response PostRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling POST request", W);

    vector<char>  body;
    const string& resource = request.get_resource();
    string fileToWrite;

    add_header(make_pair("Server", config.serverName.c_str()));

    int fileType = get_file_type(resource);
    (void) cachedPages;

    return Response(status, response_headers, body);
}
