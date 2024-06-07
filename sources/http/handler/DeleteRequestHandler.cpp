#include "DeleteRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"

Response DeleteRequestHandler::handle_request(const Request& r)
{
    LOG_DEBUG("Handling Delete request for resource: " + r.get_resource());

    _add_header("Server", cfg.serverName);
    status = r.get_status();
    vector<char> body = get_resource(r);
    return Response(status, responseHeaders, body);
}

bool endsWith(const string& str, const string& suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

const vector<char> DeleteRequestHandler::get_resource(const Request& r)
{
    string       resource = r.get_resource();
    vector<char> body;

    // Check if resource URI contains ".." and write permissions
    if (resource.find("..") != string::npos || access(resource.c_str(), W_OK) == -1)
        return make_error_body(FORBIDDEN);

    struct stat fileInfo;
    stat(resource.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode)) {
        // If resource URI does not end with "/", set status code to 409 (Conflict)
        if (!endsWith(resource, "/"))
            return make_error_body(CONFLICT);
    }

    // all checks passed, delete the resource
    if (remove(resource.c_str()) == -1)
        return make_error_body(INTERNAL_SERVER_ERROR);

    status = OK;
    return body;
}
