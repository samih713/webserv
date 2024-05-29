#include "FileType.hpp"
#include "IRequestHandler.hpp"

#ifndef REQUEST_HANDLER_BASE_HPP
#define REQUEST_HANDLER_BASE_HPP

// Base class for request handlers
class RequestHandlerBase: public IRequestHandler {
public:
    RequestHandlerBase(const ServerConfig& cfg, CachedPages& cp) : cfg(cfg), cp(cp) {};
    ~RequestHandlerBase() {};

    void add_header(const string& headerName, const string& headerValue)
    {
        responseHeaders.insert(make_pair(headerName, headerValue));
    }

    const string find_resource_type(const string& resource)
    {
        size_t extension_index = resource.find_last_of('.');
        string file_extension;
        if (extension_index != string::npos)
            file_extension = resource.substr(extension_index + 1);
        else
            file_extension = "";
        return fileTypes.find(file_extension)->second; //!
    }

    const vector<char>& get_error_body(STATUS_CODE status, CachedPages& cp)
    {
        Page& p = cp.get_error_page(status);
        add_header("Content-Type", p.contentType);
        add_header("Content-Length", ws_itoa(p.contentLength));
        return p.data;
    }

protected:
    STATUS_CODE         status;
    HeaderMap           responseHeaders;

    const ServerConfig& cfg;
    CachedPages&        cp;
};

#endif // REQUEST_HANDLER_BASE_HPP
