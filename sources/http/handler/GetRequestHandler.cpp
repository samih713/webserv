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
    CachedPages& cached, const ServerConfig& config)
{
    // status                = request.get_status();
    status                = INTERNAL_SERVER_ERROR;
    string       resource = request.get_resource();
    vector<char> body;
    // size_t       resource_size = 0;

    add_header("Server", config.serverName);

    //!
    DEBUG_MSG(">>>>>>>>>", W);
    DEBUG_MSG("Resource: " + resource, W);
    DEBUG_MSG(">>>>>>>>>", W);
    //!

    // string       defaultPage = config.root + "/";

    //* most likely an error
    if (status >= 400) {
        Page& p = cached.get_error_page(status);
        body    = p.data;
        add_header("Content-Type", p.contentType);
        add_header("Content-Length", ws_itoa(p.contentLength));
        return body;
    }

    // if (status != OK) { // TODO shouldn't just be errors
    //     Page errorPage = get_error_page(status, cached, config);
    //     // return errorPage;
    // }

    // a non implemented method is 501
    //  a URI thats too long is 414
    // if (resource == defaultPage) { //!

    //     body = cached.pages["home"].data;
    //     add_header("Content-Type", cached.pages["home"].contentType.c_str());
    //     add_header("Content-Length", ws_itoa(cached.pages["home"].contentLength));
    // }

    // ifstream resource_file(resource.c_str(), std::ios_base::binary);
    // if (resource_file.fail()) {
    //     // try to find error page
    //     Page notFound = cached.pages[];
    //     add_header("Content-Type", cached.pages["notFound"].contentType);
    //     add_header("Content-Length",
    //         ws_itoa(cached.pages["notFound"].contentLength));
    // }
    // else {
    //     string resource_type = find_resource_type(resource);
    //     if (resource_type.length() != 0)
    //         add_header("Content-Type", resource_type);
    //     if (resource.find("/cgi-bin") != string::npos) { //! cgi check again
    //         CGI    cgi(request, config);
    //         string result = cgi.execute();
    //         body          = vector<char>(result.begin(), result.end());
    //         add_header("Content-Length", ws_itoa(body.size()));
    //     }
    //     else {
    //         body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
    //             std::istreambuf_iterator<char>());
    //         resource_file.seekg(0, std::ios_base::end);
    //         resource_size = resource_file.tellg();
    //         add_header("Content-Length", ws_itoa(resource_size));
    //         resource_file.seekg(0, std::ios_base::beg);
    //     }
    // }

    /* authentication function goes here for the requested resource */
    // DEBUG_MSG("Resource '" + resource + "' : [" + ws_itoa(resource_size) + "]", W);

    // TODO caching control */

    // TODO compression/encoding

    // TODO support range requests, usefull for large files

    return body;
}


Response GetRequestHandler::handle_request(const Request& request, CachedPages& cached,
    const ServerConfig& config)
{
    DEBUG_MSG("Handling GET request ... ", B);

    // ! reply to invalid requests
    vector<char> body = get_resource(request, cached, config);
    return Response(status, responseHeaders, body);
}
