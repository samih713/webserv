#include "GetRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "webserv.hpp"

Response GetRequestHandler::handle_request(const Request& r)
{
    // LOG_INFO("Handling GET request ... "); //? commented due to many prints
    _add_header("Server", cfg.serverName);
    const vector<char>& body = get_resource(r);
    return Response(status, responseHeaders, body);
}

const vector<char> GetRequestHandler::get_resource(const Request& r)
{
    status = r.get_status();

    const string& resource     = r.get_resource();
    const string& resourcePath = r.get_resource_path();

    // return list_directory(resourcePath, resource); //! dir listing

    // default path
    if (!r.get_location_match().empty()) {
        // found matching location
    }
    else if (resourcePath == (cfg.root + "/")) {
        // TODO if the index file does not exist + autoindex is on, list dir.
        cout << "default path" << endl;

        Page& p = cp.get_page("index");
        _add_header("Content-Type", p.contentType);
        _add_header("Content-Length", ws_itoa(p.contentLength));
        return p.data;
    }

    ifstream resource_file(resourcePath.c_str(), std::ios_base::binary);
    if (resource_file.fail())
        status = NOT_FOUND; //! need more error handling, eg: forbidden, not allowed, etc

    // errors
    if (status >= 400)
        return (make_error_body(status));

    vector<char> body;
    string       resource_type = find_resource_type(resource); //! doesn't make sense
    if (resource_type.length() != 0)                           //! and what if it is zero?
        _add_header("Content-Type", resource_type);
    if (resource.find("/cgi-bin") != string::npos) {
        CGI cgi(r, cfg, cp);
        body = cgi.execute(r.cgiStatus, r.cgiReadFd,
            r.cgiChild); // ! r.fd set reference is kinda idk
        _add_header("Content-Length", ws_itoa(body.size()));
    }
    else {
        body = vector<char>((std::istreambuf_iterator<char>(resource_file)),
            std::istreambuf_iterator<char>());
        _add_header("Content-Length", ws_itoa(body.size()));
    }

    /* authentication function goes here for the requested resource */
    // DEBUG_MSG("Resource '" + resource + "' : [" + ws_itoa(resource_size) + "]", W);

    // TODO caching control */

    // TODO compression/encoding

    // TODO support range requests, usefull for large files

    return body;
}
