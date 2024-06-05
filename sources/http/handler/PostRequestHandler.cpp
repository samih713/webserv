#include "PostRequestHandler.hpp"

Response PostRequestHandler::handle_request(const Request& r)
{
    LOG_DEBUG("Handling POST request for resource: " + r.get_resource());

    vector<char>  body;
    const string& resource = r.get_resource();
    string        fileToWrite;

    _add_header("Server", cfg.serverName);

    FileType fileType = get_file_type(resource);
    if (fileType == NO_EXIST) {
        // resource does not exist so create it
    }
    else if (fileType == NO_PERM) {
        // resource exists but no permission to write
        // return 403 Forbidden
    }
    else if (fileType == REG_FILE) {
        // resource exists and is a regular file
        // append to file
    }
    else if (fileType == DIR) {
        // resource exists and is a directory
        //
    }
    else {
        // unexpected error
    }
    (void) fileType;

    (void) cp;

    return Response(status, responseHeaders, body);
}
