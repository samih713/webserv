#include "FileType.hpp"
#include "IRequestHandler.hpp"

#ifndef REQUEST_HANDLER_BASE_HPP
#define REQUEST_HANDLER_BASE_HPP

// Base class for request handlers
class RequestHandlerBase: public IRequestHandler {
public:
    RequestHandlerBase(const ServerConfig& cfg) : cfg(cfg), cp(cfg.cp) {};
    ~RequestHandlerBase() {};

    void _add_header(const string& headerName, const string& headerValue)
    {
        responseHeaders.insert(make_pair(headerName, headerValue));
    }

    const string find_resource_type(const string& resource)
    {
        size_t extensionPos = resource.find_last_of('.');
        if (extensionPos == string::npos)
            return ""; // no matching extension

        string fileExtension = resource.substr(extensionPos + 1);

        HeaderMap::const_iterator fileTypeItr = fileTypes.find(fileExtension);
        if (fileTypeItr == fileTypes.end())
            return "";
        return fileTypeItr->second;
    }

    const vector<char>& make_error_body(STATUS_CODE _status)
    {
		status = _status;
        Page& p = cp->get_error_page(status);

        _add_header("Content-Type", p.contentType);
        _add_header("Content-Length", ws_itoa(p.contentLength));
        return p.data;
    }

protected:
    STATUS_CODE status;
    HeaderMap   responseHeaders;

    const ServerConfig& cfg;
    CachedPages*        cp;
};

#endif // REQUEST_HANDLER_BASE_HPP
