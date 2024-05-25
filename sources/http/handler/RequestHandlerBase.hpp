#include "IRequestHandler.hpp"
#include "FileType.hpp"

#ifndef REQUESTHANDLERBASE_HPP
#define REQUESTHANDLERBASE_HPP

// Base class for request handlers
class RequestHandlerBase: public IRequestHandler {
public:
    RequestHandlerBase() {};
    ~RequestHandlerBase() {};
    void add_header(pair<string, string> header_field)
    {
        response_headers.push_back(header_field);
    }
    //! ideally this should be done in Request itself
    const string find_resource_type(const string& resource)
    {
        size_t extension_index = resource.find_last_of('.');
        string file_extension;
        if (extension_index != string::npos)
            file_extension = resource.substr(extension_index + 1);
        else
            file_extension = "";
         //! need to protect against fail case
        return fileTypes.find(file_extension)->second;
    }

protected:
    STATUS_CODE status;
    vsp         response_headers;
};

#endif // REQUESTHANDLERBASE_HPP
