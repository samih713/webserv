#include "DeleteRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "FileType.hpp"
#include "debug.hpp"

DeleteRequestHandler::DeleteRequestHandler()
{
    DEBUG_MSG("DeleteRequestHandler constructor called", B);
}

DeleteRequestHandler::~DeleteRequestHandler()
{
    DEBUG_MSG("DeleteRequestHandler destructor called", B);
};

// parse accepted formats
inline const string find_resource_type(const string& resource)
{
    size_t extension_index = resource.find_last_of('.');
    string file_extension;
    if (extension_index != string::npos)
        file_extension = resource.substr(extension_index + 1);
    else
        file_extension = "";
    return (fileTypes.find(file_extension)->second);
}

bool restrict_path(const string& resourcePath)
{
    size_t pos = resourcePath.find("..");

    if (pos != string::npos)
        return (true);
    else
        return (false);
}

bool endsWith(const string& str, const string& suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}


const vector<char> DeleteRequestHandler::get_resource(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    HeaderMap requestHeaders = request.get_headers();
    string    resource       = request.get_resource();


    add_header(make_pair<string, string>("Server", config.serverName.c_str()));

    if (restrict_path(resource)) {
        status = FORBIDDEN;
        add_header(make_pair<string, string>("Content-Type",
            cachedPages->notFound.contentType.c_str()));
        add_header(make_pair<string, string>("Content-Length",
            ws_itoa(cachedPages->notFound.contentLength)));
        body = cachedPages->notFound.data;
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
                add_header(make_pair<string, string>("Content-Type",
                    cachedPages->notFound.contentType.c_str()));
                add_header(make_pair<string, string>("Content-Length",
                    ws_itoa(cachedPages->notFound.contentLength)));
                body = cachedPages->notFound.data;
            }
        }
        else

            if (!endsWith(resource, "/"))
        {
            // If resource URI does not end with "/", set status code to 409 (Conflict)
            status = CONFLICT;
        }
        else {
            // Check write permissions
            if (access(resource.c_str(), W_OK) == 0) {
                // Write permission granted, attempt to delete resource
                if (remove(resource.c_str()) != 0) {
                    // Error deleting resource
                    DEBUG_MSG("Resource '" + resource + "' : [ Error deleting resource ]",
                        R);
                    status = INTERNAL_SERVER_ERROR; // Internal Server Error
                }
                else {
                    // Deletion successful
                    DEBUG_MSG("Resource '" + resource + "' : [ Deleted ]", W);
                    status = NO_CONTENT;
                    add_header(make_pair<string, string>("Content-Type",
                        cachedPages->notFound.contentType.c_str()));
                    add_header(make_pair<string, string>("Content-Length",
                        ws_itoa(cachedPages->notFound.contentLength)));
                    body = cachedPages->notFound.data;
                }
            }
            else {
                // No write permission
                cerr << "Write permission is not granted for " << resource << endl;
                status = FORBIDDEN; // Forbidden
            }
        }
    }
    /* authentication function goes here for the requested resource */
    DEBUG_MSG("Resource '" + resource, W);
    return body;
}


Response DeleteRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling Delete request ... ", B);

    HeaderMap request_headers = request.get_headers();
    body                      = get_resource(request, cachedPages, config);
    return Response(status, response_headers, body);
}
