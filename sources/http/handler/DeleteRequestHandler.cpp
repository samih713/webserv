#include "DeleteRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "debug.hpp"

DeleteRequestHandler::DeleteRequestHandler()
{
    DEBUG_MSG("DeleteRequestHandler constructor called", B);
}

DeleteRequestHandler::~DeleteRequestHandler()
{
    DEBUG_MSG("DeleteRequestHandler destructor called", B);
};

bool endsWith(const string& str, const string& suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

const vector<char> DeleteRequestHandler::get_resource(const Request& request,
    CachedPages& cached, const ServerConfig& config)
{
    (void) cached;
    string       resource = request.get_resource();
    vector<char> body;

    add_header("Server", config.serverName);

    if (resource.find("..") != string::npos) {
        status = FORBIDDEN;
        // add_header("Content-Type", cached.notFound.contentType);
        // add_header("Content-Length", ws_itoa(cached.notFound.contentLength));
        // body = cached.notFound.data;
        DEBUG_MSG("Restricted", W);
    }
    else {
        string resource_type = find_resource_type(resource);
        if (resource_type.length() != 0) {
            if (remove(resource.c_str()) != 0) {
                cerr << "Error deleting resource: " << resource << endl;
                DEBUG_MSG("Resource '" + resource + "' : [ Error deleting resource ]", R);
                status = OK;
            }
            else {
                DEBUG_MSG("Resource '" + resource + "' : [ Deleted ]", W);
                status = NOT_FOUND;
                // add_header("Content-Type", cached.notFound.contentType);
                // add_header("Content-Length", ws_itoa(cached.notFound.contentLength));
                // body = cached.notFound.data;
            }
        }
        else {
            // If resource URI does not end with "/", set status code to 409 (Conflict)
            if (!endsWith(resource, "/"))
                status = CONFLICT;
            else {
                // Check write permissions
                if (access(resource.c_str(), W_OK) == 0) {
                    // Write permission granted, attempt to delete resource
                    if (remove(resource.c_str()) != 0) {
                        DEBUG_MSG("Resource '" + resource +
                                      "' : [ Error deleting resource ]",
                            R);
                        status = INTERNAL_SERVER_ERROR; // Internal Server Error
                    }
                    else {
                        // Deletion successful
                        DEBUG_MSG("Resource '" + resource + "' : [ Deleted ]", W);
                        status = NO_CONTENT;
                        // add_header("Content-Type",
                        //     cached.notFound.contentType); //? why
                        // add_header("Content-Length",
                        //     ws_itoa(cached.notFound.contentLength)); //? why
                        // body = cached.notFound.data;                 //? why
                    }
                }
                else {
                    // No write permission
                    cerr << "Write permission is not granted for " << resource << endl;
                    status = FORBIDDEN; // Forbidden
                }
            }
        }
    }
    /* authentication function goes here for the requested resource */
    DEBUG_MSG("Resource '" + resource, W);
    return body;
}

Response DeleteRequestHandler::handle_request(const Request& request, CachedPages& cached,
    const ServerConfig& config)
{
    DEBUG_MSG("Handling Delete request ... ", B);

    vector<char> body = get_resource(request, cached, config);
    return Response(status, responseHeaders, body);
}
