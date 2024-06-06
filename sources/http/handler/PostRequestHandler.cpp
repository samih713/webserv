#include "PostRequestHandler.hpp"

Response PostRequestHandler::handle_request(const Request& r)
{
    DEBUG_MSG("Handling POST request for " << r.get_resource(), W);

    // TODO need to check if the body is exceeding max body size
    // TODO need to check type of content

    status = r.get_status();
    _add_header("Server", cfg.serverName);

    vector<char> body = process_data(r);
    return Response(status, responseHeaders, body);
}

vector<char> PostRequestHandler::process_data(const Request& r)
{
    const string& resource    = r.get_resource();
    const string& requestBody = r.get_body();
    vector<char>  responseBody;

    // for uploads, check if the location block allows uploads

    if (requestBody.empty()) // no data to write
        return make_error_body(BAD_REQUEST);

    // append to file
    ofstream outputFile(resource.c_str(), std::ios_base::app);
    if (!outputFile.is_open()) // failed to open file
        return make_error_body(INTERNAL_SERVER_ERROR);

    // need to check if file is too big (return 413 if so)
    // need to check if the file type is allowed (return 415 if not)
    // need to handle CGI POST requests
    //? need to parse the request body
    outputFile << requestBody;
    outputFile.close();

    status = OK;
    //! build response body
    string successMsg = "POST request was successful.";
    responseBody.assign(successMsg.begin(), successMsg.end());
    _add_header("Content-Length", ws_itoa(responseBody.size()));
    _add_header("Content-Type", "text/html;");

    return responseBody;
}
