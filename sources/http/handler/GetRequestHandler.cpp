#include "GetRequestHandler.hpp"
#include "debug.hpp"
#include "FileType.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"

GetRequestHandler::GetRequestHandler()
{
    DEBUG_MSG("GetRequestHandler constructor called", B);
}

GetRequestHandler::~GetRequestHandler()
{
    DEBUG_MSG("GetRequestHandler destructor called", B);
};

// parse accepted formats
inline const string find_resource_type(const string &resource)
{
    size_t extension_index = resource.find_last_of('.');
    string file_extension;
    if (extension_index != string::npos)
        file_extension = resource.substr(extension_index + 1);
    else
        file_extension = "";
    return (fileTypes.find(file_extension)->second);
}

bool check_cgi_request(const string &res)
{
	string filetype;

	filetype = find_resource_type(res);
	if(filetype == "bash" || filetype == "python")
		return true;
	else
		return false;
}


const vector<char> GetRequestHandler::get_resource(const Request      &request,
                                                   const CachedPages  *cachedPages,
                                                   const ServerConfig &config)
{
    vsp          requestHeaders = request.get_headers();
    string       resource = request.get_resource();
    vector<char> body;


    // TODO find a better way to include server name
    add_header(std::make_pair<string, string>("Server", "The Wired"));

    /* need to build resource location from the
     * root directory in server configuration
     * */

    ifstream resource_file;
    size_t   resource_size = 0;
    status = OK;
    if (resource == "/")
    {
        body = cachedPages->home.data;
        add_header(std::make_pair<string, string>("Content-Type",
                                                  cachedPages->home.contentType));

        // remove
        resource_size = cachedPages->home.contentLength;

        add_header(std::make_pair<string, string>(
            "Content-Length", ws_itoa(cachedPages->home.contentLength)));
    }
    else
    {
        resource = config.serverRoot + resource;
        resource_file.open(resource.c_str(), std::ios_base::binary);
        if (resource_file.fail())
        {
            status = NOT_FOUND;
            add_header(std::make_pair<string, string>("Content-Type",
                                                      cachedPages->notFound.contentType));
            add_header(std::make_pair<string, string>(
                "Content-Length", ws_itoa(cachedPages->notFound.contentLength)));
            body = cachedPages->notFound.data;
        }
        else
        {
			if(check_cgi_request(resource))
			{
                Cgi cgi(request);
				string strv;

				strv = cgi.execute();
                body = vector<char>(strv.begin(), strv.end());
			}
			else
				body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
                                std::istreambuf_iterator<char>());

            // content type
            string resource_type = find_resource_type(resource);
            if (resource_type.length() != 0)
                add_header(std::make_pair<string, string>("Content-Type", resource_type));
            // content length
            resource_file.seekg(0, std::ios_base::end);
            resource_size = resource_file.tellg();
            add_header(
                std::make_pair<string, string>("Content-Length", ws_itoa(resource_size)));
            resource_file.seekg(0, std::ios_base::beg);
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


Response GetRequestHandler::handle_request(const Request      &request,
                                           const CachedPages  *cachedPages,
                                           const ServerConfig &config)
{
    DEBUG_MSG("Handling get request ... ", B);

    vsp request_headers = request.get_headers();
    body = get_resource(request, cachedPages, config);
    return Response(status, response_headers, body);
}
