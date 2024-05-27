#include "GetRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "debug.hpp"
#include "webserv.hpp"

GetRequestHandler::GetRequestHandler()
{
    DEBUG_MSG("GetRequestHandler constructor called", B);
}

GetRequestHandler::~GetRequestHandler()
{
    DEBUG_MSG("GetRequestHandler destructor called", B);
};

// TODO resource handling for get-requests, is broken
const vector<char> GetRequestHandler::get_resource(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    string       resource    = request.get_resource();
    string       defaultPage = config.root + "/";
    vector<char> body;

    // add_general_headers()
    add_header(make_pair("Server", config.serverName.c_str()));

    size_t resource_size = 0;

    status = request.get_status();
    // if (status != OK) handle other bad cases
    //     return body;
    // a non implemented method is 501
    //  a URI thats too long is 414
    if (resource == defaultPage) { //!
        body = cachedPages->home.data;
        add_header(make_pair("Content-Type", cachedPages->home.contentType.c_str()));
        add_header(make_pair("Content-Length", ws_itoa(cachedPages->home.contentLength)));
    }
    else {
        ifstream resource_file(resource.c_str(), std::ios_base::binary);
        if (resource_file.fail()) {
            status = NOT_FOUND;
            add_header(
                make_pair("Content-Type", cachedPages->notFound.contentType.c_str()));
            add_header(make_pair("Content-Length",
                ws_itoa(cachedPages->notFound.contentLength)));
            body = cachedPages->notFound.data;
        }
        else {
            string resource_type = find_resource_type(resource);
            if (resource_type.length() != 0)
                add_header(make_pair("Content-Type", resource_type.c_str()));
            if (resource.find("/cgi-bin") != string::npos) { //! cgi check again
                CGI    cgi(request, config);
                string result = cgi.execute();
                body          = vector<char>(result.begin(), result.end());
                add_header(make_pair("Content-Length", ws_itoa(body.size())));
            }
            else {
                body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
                    std::istreambuf_iterator<char>());
                resource_file.seekg(0, std::ios_base::end);
                resource_size = resource_file.tellg();
                add_header(make_pair("Content-Length", ws_itoa(resource_size)));
                resource_file.seekg(0, std::ios_base::beg);
            }
        }
    }
    /* authentication function goes here for the requested resource */
    DEBUG_MSG("Resource '" + resource + "' : [" + ws_itoa(resource_size) + "]", W);

    /* // TODO caching control */

    /* // TODO compression/encoding
     * */

    /* // TODO support range requests, usefull for large files
     * */
    return body;
}


Response GetRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling GET request ... ", B);

    // ! reply to invalid requests
    vector<char> body = get_resource(request, cachedPages, config);
    return Response(status, responseHeaders, body);
}
