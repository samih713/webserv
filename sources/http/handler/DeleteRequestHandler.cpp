#include "DeleteRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"

Response DeleteRequestHandler::handle_request(const Request& r)
{
    DEBUG_MSG("Handling Delete request ... ", B);

    _add_header("Server", cfg.serverName);

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
    if (resource.find("..") != string::npos || access(resource.c_str(), W_OK) == -1) {
        LOG_INFO("DELETE: Resource '" + resource + "' : [ Forbidden ]");
        status = FORBIDDEN;
        return (make_error_body(status, cp));
    }

    //? why is this needed for DELETE
    // string resource_type = find_resource_type(resource);
    // if (resource_type.length() != 0)
    //     _add_header("Content-Type", resource_type);

    // If resource URI does not end with "/", set status code to 409 (Conflict)
    if (!endsWith(resource, "/")) {
        LOG_ERROR("DELETE: Resource '" + resource + "' : [ URI does not end with '/' ]");
        status = CONFLICT;
        return (make_error_body(status, cp));
    }

    // all checks passed, delete the resource
    if (remove(resource.c_str()) == -1) {
        LOG_INFO("DELETE: Resource '" + resource + "' : [ Error deleting resource ]");
        status = INTERNAL_SERVER_ERROR;
        return (make_error_body(status, cp));
    }
    LOG_INFO("DELETE: Resource '" + resource + "' : [ Deleted ]");
    status = OK;
    return make_error_body(status, cp); // Return 200 OK
}
