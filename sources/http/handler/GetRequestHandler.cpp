#include "GetRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "webserv.hpp"

Response GetRequestHandler::handle_request(const Request& r)
{
    // LOG_INFO("Handling GET request ... "); //? commented due to many prints
    const vector<char>& body = get_resource(r);
    return Response(status, responseHeaders, body);
}

const vector<char> GetRequestHandler::get_resource(const Request& r)
{
    status          = r.get_status();
    string resource = r.get_resource();

    vector<char> body;

    _add_header("Server", cfg.serverName);

    // default path
    if (resource == (cfg.root + "/")) {
        Page& p = cp.get_page("index");
        _add_header("Content-Type", p.contentType);
        _add_header("Content-Length", ws_itoa(p.contentLength));
        return p.data;
    }

    ifstream resource_file(resource.c_str(), std::ios_base::binary);
    if (resource_file.fail())
        status = NOT_FOUND; //! need more error handling, eg: forbidden, not allowed, etc

    // errors
    if (status >= 400)
        return (make_error_body(status, cp));

    string resource_type = find_resource_type(resource); //! doesn't make sense
    if (resource_type.length() != 0)
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

    // TODO caching control */

    // TODO compression/encoding

    // TODO support range requests, usefull for large files

    return body;
}
