#include "PostRequestHandler.hpp"

PostRequestHandler::PostRequestHandler()
{
    DEBUG_MSG("PostRequestHandler constructor called", B);
}

PostRequestHandler::~PostRequestHandler()
{
    DEBUG_MSG("PostRequestHandler destructor called", B);
}

Response PostRequestHandler::handle_request(const Request& request,
    const CachedPages* cachedPages, const ServerConfig& config)
{
    DEBUG_MSG("Handling POST request", W);

    vector<char>  responseBody;
    const string& resource = request.get_resource();
    string        fileToWrite;

    // TODO need to check if the body is exceeding max body size
    // TODO need to check type of content

    add_header(make_pair("Server", config.serverName.c_str()));

    cout << "Resource: " << resource << endl;

    FileType fileType = get_file_type(resource);
    // if (fileType == NO_EXIST) {
    //     cout << "Resource does not exist" << endl;
    //     // resource does not exist so create it
    // }
    // else 
    if (fileType == NO_PERM) {
        cout << "No permission to write" << endl;
        // resource exists but no permission to write
        // return 403 Forbidden
    }
    else if (fileType == REG_FILE || fileType == NO_EXIST) {
        cout << "Resource exists and is a regular file" << endl;
        // resource exists and is a regular file
        // append to file
        ofstream outputFile(resource.c_str(), std::ios_base::app);
        if (!outputFile.is_open()) {
            // cant open output file
            //! http code 500 = INTERNAL_SERVER_ERROR
            THROW_EXCEPTION_WITH_INFO("POST: cannot open file"); //!
        }
        outputFile << request.get_body();
        outputFile.close();

        status = OK;
        //! build request body
        string successMsg = "POST request was successful.";
        responseBody.assign(successMsg.begin(), successMsg.end());
        add_header(make_pair("Content-Length", ws_itoa(responseBody.size())));
        add_header(make_pair("Content-Type", "text/html;"));
    }
    else if (fileType == DIR) {
        cout << "Resource exists and is a directory" << endl;
        // resource exists and is a directory
    }
    else {
        cout << "Unexpected error" << endl;
        // unexpected error
    }

    (void) cachedPages;

    return Response(status, response_headers, responseBody);
}
