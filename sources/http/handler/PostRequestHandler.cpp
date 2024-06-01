#include "PostRequestHandler.hpp"

Response PostRequestHandler::handle_request(const Request& r)
{
    DEBUG_MSG("Handling POST request", W);

    // TODO need to check if the body is exceeding max body size
    // TODO need to check type of content

    status = request.get_status();
    _add_header("Server", cfg.serverName);

    vector<char> body = process_data(request);
    return Response(status, response_headers, body);
}

vector<char> PostRequestHandler::process_data(const Request& r)
{
    const string& resource    = r.get_resource();
    const string& requestBody = r.get_body();
    vector<char>  responseBody;

    // for uploads, check if the location block allows uploads

    if (requestBody.empty()) { // no data to write
        status = BAD_REQUEST;
        responseBody.push_back('b'); //! placeholder
        return responseBody;
    }

    // append to file
    ofstream outputFile(resource.c_str(), std::ios_base::app);
    if (!outputFile.is_open()) {
        // cant open output file
        status = INTERNAL_SERVER_ERROR;
        responseBody.push_back('i'); //! placeholder
        return responseBody;
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
    add_header(make_pair("Content-Length", ws_itoa(responseBody.size())));
    add_header(make_pair("Content-Type", "text/html;"));

    return responseBody;
}
