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

bool restrict_path(const string& resourcePath)
{
    size_t pos = resourcePath.find("..");

    if (pos != std::string::npos)
        return (true);
    else
        return (false);
}


    // TODO resource handling for get-requests, is broken
const vector<char> DeleteRequestHandler::get_resource(const Request& request,
        const CachedPages* cachedPages, const ServerConfig& config)
{
    vsp          requestHeaders = request.get_headers();
    string       resource       = request.get_resource();
   

	(void)cachedPages;
    add_header(make_pair<string, string>("Server", config.serverName.c_str()));

    if (restrict_path(resource)) {
        status = NOT_FOUND;
		add_header(make_pair<string, string>("Content-Type",
                cachedPages->notFound.contentType.c_str()));
        add_header(make_pair<string, string>("Content-Length",
            ws_itoa(cachedPages->notFound.contentLength)));
		body = cachedPages->notFound.data;
		 DEBUG_MSG("Restricted", W);
    }
    else {

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
    /* authentication function goes here for the requested resource */
    DEBUG_MSG("Resource '" + resource, W);
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
