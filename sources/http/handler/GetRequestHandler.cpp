#include "GetRequestHandler.hpp"
#include "CachedPages.hpp"
#include "Cgi.hpp"
#include "webserv.hpp"

Response GetRequestHandler::handle_request(const Request& r)
{
    // LOG_INFO("Handling GET request ... "); //? commented due to many prints
    _add_header("Server", cfg.serverName);
    status                   = r.get_status();
    const vector<char>& body = get_resource(r);
    return Response(status, responseHeaders, body);
}

const vector<char> GetRequestHandler::get_resource(const Request& r)
{
    const string& resource      = r.get_resource();
    const string& resourcePath  = r.get_resource_path();
    const string& locationMatch = r.get_location_match();

    string defaultPath = cfg.root + "/";
    string index       = defaultPath + cfg.indexFile;
    bool   autoindex   = cfg.autoindex;

    if (!locationMatch.empty()) {
        const Location& location = cfg.locations.at(locationMatch);

        autoindex   = location.autoindex;
        defaultPath = location.root + "/";
        index       = defaultPath + location.indexFile;
        cp->set_index_page(index);
    }

    vector<char> body;
    if (!locationMatch.empty() && locationMatch == "/cgi-bin") {
        CGI cgi(r, cfg, *cp);
        body = cgi.execute(r.cgiStatus, r.cgiReadFd, r.cgiChild);
        _add_header("Content-Length", ws_itoa(body.size()));
        status = OK;
        return body;
    }

    struct stat  fileInfo;
    stat(resourcePath.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode)) {
        ifstream indexFile(index.c_str(), std::ios_base::binary);
        if (indexFile.good()) {
            Page& p = cp->get_page(index);
            _add_header("Content-Type", p.contentType);
            _add_header("Content-Length", ws_itoa(p.contentLength));
            return p.data;
        }
        else if (autoindex) {
            body = list_directory(resourcePath, resource);
            _add_header("Content-Type", "text/html");
            _add_header("Content-Length", ws_itoa(body.size()));
            return body;
        }
        return make_error_body(NOT_FOUND);
    }

    ifstream resourceFile(resourcePath.c_str(), std::ios_base::binary);
    if (resourceFile.fail())
        return make_error_body(NOT_FOUND);
    else if (access(resourcePath.c_str(), R_OK) == -1)
        return make_error_body(FORBIDDEN);

    string resource_type = find_resource_type(resource);
    if (resource_type.length() == 0)
        return make_error_body(UNSUPPORTED_MEDIA_TYPE);

    _add_header("Content-Type", resource_type);


    body = vector<char>((std::istreambuf_iterator<char>(resourceFile)),
        std::istreambuf_iterator<char>());
    _add_header("Content-Length", ws_itoa(body.size()));
    status = OK;
    return body;
}
