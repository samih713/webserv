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

    FileType fileType = get_file_type(resource);
    if (fileType == NO_EXIST) {
        // resource does not exist so create it
    } else if (fileType == NO_PERM) {
        // resource exists but no permission to write
        // return 403 Forbidden
    } else if (fileType == REG_FILE) {
        // resource exists and is a regular file
        // append to file
    } else if (fileType == DIR) {
        // resource exists and is a directory
        //
    } else {
        // unexpected error
    }
    (void) fileType;

    (void) cachedPages;

    return Response(status, responseHeaders, body);
}
