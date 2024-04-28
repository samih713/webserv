#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "FileType.hpp"
#include "DeleteRequestHandler.hpp"
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

// Function to delete a resource if found
bool deleteResource(const string& resourcePath)
{
    if (remove(resourcePath.c_str()) != 0) {
        cerr << "Error deleting resource: " << resourcePath << endl;
        return false;
    }
    else {
        cout << "Resource deleted successfully: " << resourcePath << endl;
        return true;
    }
}


// TODO resource handling for get-requests, is broken
const vector<char> DeleteRequestHandler::get_resource(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    vsp          requestHeaders = request.get_headers();
    string       resource       = request.get_resource();
    string       defaultPage    = config.serverRoot + "/";
    vector<char> body;


    add_header(make_pair<string, string>("Server", config.serverName.c_str()));

    ifstream resource_file;
    size_t   resource_size = 0;
    status                 = OK;
    if (resource == defaultPage) {
        body = cachedPages->home.data;
        add_header(make_pair<string, string>("Content-Type",
            cachedPages->home.contentType.c_str()));

        add_header(make_pair<string, string>("Content-Length",
            ws_itoa(cachedPages->home.contentLength)));
    }
    else {
        resource_file.open(resource.c_str(), std::ios_base::binary);
        if (resource_file.fail()) {
            status = NOT_FOUND;
            add_header(make_pair<string, string>("Content-Type",
                cachedPages->notFound.contentType.c_str()));
            add_header(make_pair<string, string>("Content-Length",
                ws_itoa(cachedPages->notFound.contentLength)));
            body = cachedPages->notFound.data;
        }
        else {
            string resource_type = find_resource_type(resource);
            if (resource_type.length() != 0)
			{
                add_header(
                    make_pair<string, string>("Content-Type", resource_type.c_str()));
			}
			body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
				std::istreambuf_iterator<char>());
			resource_file.seekg(0, std::ios_base::end);
			resource_size = resource_file.tellg();
			add_header(
				make_pair<string, string>("Content-Length", ws_itoa(resource_size)));
			resource_file.seekg(0, std::ios_base::beg);
			if (deleteResource(resource))
				status = OK;
			else
				status = NOT_FOUND;
                // content type
                // content length
        }
    }
    /* authentication function goes here for the requested resource */
    DEBUG_MSG("Resource '" + resource + "' : [" + ws_itoa(resource_size) + "]", W);
    /* caching control */

    /* compression/encoding
     * */

    /* support range requests, usefull for large files
     * */
    return body;
}


Response DeleteRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling Delete request ... ", B);

    vsp request_headers = request.get_headers();
    body                = get_resource(request, cachedPages, config);
    return Response(status, response_headers, body);
}
