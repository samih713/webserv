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
    // const string& resource      = r.get_resource();
    const string& resourcePath  = r.get_resource_path();
    const string& requestBody   = r.get_body();
    const string& locationMatch = r.get_location_match();


    vector<char> responseBody;
    // for uploads, check if the location block allows uploads
    if (!locationMatch.empty() && locationMatch == "/upload") {
        handle_upload();
    }

    if (requestBody.empty()) // no data to write
        return make_error_body(BAD_REQUEST);

    ofstream outputFile(resourcePath.c_str(), std::ios_base::app);
    if (outputFile.fail()) // failed to open file
        return make_error_body(INTERNAL_SERVER_ERROR);

    // need to check if file is too big (return 413 if so)
    // need to check if the file type is allowed (return 415 if not)
    if (!parse_request_body(requestBody))
        return make_error_body(BAD_REQUEST);

    outputFile.close();

    status = OK;
    return responseBody;
}

bool PostRequestHandler::parse_request_body(const string& body)
{
    map<string, string> form;
    istringstream       iss(body);
    string              keyValuePair;
    while (std::getline(iss, keyValuePair, '&')) {
        istringstream keyValueStream(keyValuePair);
        string        key, value;
        if (std::getline(std::getline(keyValueStream, key, '='), value)) {
            if (!key.empty() && !value.empty())
                form.insert(make_pair(key, value));
            else
                return false;
        }
    }
    return true;
}

void handle_upload()
{
    // ...
}
