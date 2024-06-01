#include "PostRequestHandler.hpp"

Response PostRequestHandler::handle_request(const Request& r)
{
    DEBUG_MSG("Handling POST request", W);

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
        return make_error_body(BAD_REQUEST, cp);

    // append to file
    cout << resource << endl;
    ofstream outputFile(resource.c_str(), std::ios_base::app);
    if (!outputFile.is_open()) // failed to open file
    {
        cout << "Failed to open file\n";
        return make_error_body(INTERNAL_SERVER_ERROR, cp);
    }

    // need to check if file is too big (return 413 if so)
    // need to check if the file type is allowed (return 415 if not)
    // need to handle CGI POST requests
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
