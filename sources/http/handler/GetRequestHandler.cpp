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

// bool check_cgi_request(const string &res)
// {
// 	string filetype;

// 	filetype = find_resource_type(res);
// 	if(filetype == "bash" || filetype == "python")
// 		return true;
// 	else
// 		return false;
// }

bool check_cgi_request(string res)
{
    // Find the position of the word in the string
    size_t pos = res.find("cgi-bin");

    if (pos != std::string::npos)
        return(true);
    else
        return(false);


}

//fuction to get url without the query string
string geturis(string res)
{
	string resn;
	size_t qu;

	qu = res.find('?', 0); 
	resn = res.substr(0, qu);
	//result = const_cast<char *>(res.substr(0,qu).c_str());
	cout << resn << endl;
	cout<< "test" << qu <<endl;
    return (resn);
}


// TODO resource handling for get-requests, is broken
const vector<char> GetRequestHandler::get_resource(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    vsp          requestHeaders = request.get_headers();
    string       resource       = request.get_resource();
    string       defaultPage    = config.serverRoot + "/";
    vector<char> body;
	string		plain_res		= geturis(resource);


    add_header(make_pair<string, string>("Server", config.serverName.c_str()));

    ifstream resource_file;
    size_t   resource_size = 0;
    status                 = OK;
    if (plain_res == defaultPage) {
        body = cachedPages->home.data;
        add_header(
            make_pair<string, string>("Content-Type", cachedPages->home.contentType.c_str()));

        add_header(make_pair<string, string>("Content-Length",
            ws_itoa(cachedPages->home.contentLength)));
    }
    else {
        resource_file.open(plain_res.c_str(), std::ios_base::binary);
        if (resource_file.fail()) {
            status = NOT_FOUND;
            add_header(make_pair<string, string>("Content-Type",
                cachedPages->notFound.contentType.c_str()));
            add_header(make_pair<string, string>("Content-Length",
                ws_itoa(cachedPages->notFound.contentLength)));
            body = cachedPages->notFound.data;
        }
        else {
            string resource_type = find_resource_type(plain_res);
            if (resource_type.length() != 0)
                add_header(make_pair<string, string>("Content-Type", resource_type.c_str()));
            if (check_cgi_request(plain_res)) {
                Cgi    cgi(request, config);
                string result;

                result = cgi.execute();
                body   = vector<char>(result.begin(), result.end());
                add_header(
                    make_pair<string, string>("Content-Length", ws_itoa(body.size())));
            }
            else {
                body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
                    std::istreambuf_iterator<char>());
                resource_file.seekg(0, std::ios_base::end);
                resource_size = resource_file.tellg();
                add_header(
                    make_pair<string, string>("Content-Length", ws_itoa(resource_size)));
                resource_file.seekg(0, std::ios_base::beg);
            }
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


Response GetRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling get request ... ", B);

    vsp request_headers = request.get_headers();
    body                = get_resource(request, cachedPages, config);
    return Response(status, response_headers, body);
}
