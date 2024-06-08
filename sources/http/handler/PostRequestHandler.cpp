#include "PostRequestHandler.hpp"
#include "Cgi.hpp"

Response PostRequestHandler::handle_request(const Request& r)
{
    LOG_DEBUG("Handling POST request for resource: " + r.get_resource());
    _add_header("Server", cfg.serverName);
    status            = r.get_status();
    vector<char> body = process_data(r);
    return Response(status, responseHeaders, body);
}

vector<char> PostRequestHandler::process_data(const Request& r)
{
    const string& resourcePath  = r.get_resource_path();
    const string& requestBody   = r.get_body();
    const string& locationMatch = r.get_location_match();

    if (requestBody.empty()) // no data to write
        return make_error_body(BAD_REQUEST);

    vector<char> responseBody;
    if (!locationMatch.empty() && locationMatch == "/upload") {
        responseBody = handle_upload(r, cfg.locations.at("/upload").root);
        _add_header("Content-Length", ws_itoa(responseBody.size()));
        return responseBody;
    }
    else if (!locationMatch.empty() && locationMatch == "/cgi-bin") {
        CGI cgi(r, cfg, *cp);
        responseBody = cgi.execute(r.cgiStatus, r.cgiReadFd, r.cgiChild);
        _add_header("Content-Length", ws_itoa(responseBody.size()));
        return responseBody;
    }

    struct stat fileInfo;
    stat(resourcePath.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode))
        return make_error_body(NOT_IMPLEMENTED);
    else if (find_resource_type(r.get_resource()).length() == 0)
        return make_error_body(UNSUPPORTED_MEDIA_TYPE);

    // append to file
    ofstream outputFile(resourcePath.c_str(), std::ios_base::app);
    if (outputFile.fail()) // failed to open file
        return make_error_body(INTERNAL_SERVER_ERROR);

    outputFile << requestBody;
    outputFile.close();

    status = OK;
    return responseBody;
}

vector<char> PostRequestHandler::handle_upload(const Request& r, const string& dirPath)
{
    HeaderMap::const_iterator itr = r.get_headers().find("content-type");
    if (itr == r.get_headers().end())
        return make_error_body(BAD_REQUEST);

    string contentType = itr->second;
    // Find the boundary parameter
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == string::npos)
        return make_error_body(BAD_REQUEST);

    // Extract the boundary value (9 is the length of "boundary=")
    string boundary = contentType.substr(boundaryPos + 9);
    if (boundary.empty())
        return make_error_body(BAD_REQUEST);

    // Add the -- prefix to the boundary
    string boundaryDelimiter    = "--" + boundary;
    string endBoundaryDelimiter = boundaryDelimiter + "--";

    // Get the body of the request
    string body = r.get_body();

    // Find the start of the first part
    size_t startPos = body.find(boundaryDelimiter);
    if (startPos == string::npos)
        return make_error_body(BAD_REQUEST);
    // Move past the boundary delimiter and the following CRLF
    startPos += boundaryDelimiter.length() + 2;

    // Find the end of the first part
    size_t endPos = body.find(boundaryDelimiter, startPos);
    if (endPos == string::npos)
        return make_error_body(BAD_REQUEST);

    // Extract the headers and content of the part
    string part = body.substr(startPos, endPos - startPos);

    // Find the end of the headers (CRLF CRLF)
    size_t headerEndPos = part.find("\r\n\r\n");
    if (headerEndPos == string::npos)
        return make_error_body(BAD_REQUEST);

    // Extract the headers and content
    string bodyHeaders = part.substr(0, headerEndPos);
    string fileContent = part.substr(headerEndPos + 4); // Move past the CRLF CRLF

    // Find the filename in the headers
    size_t filenamePos = bodyHeaders.find("filename=\"");
    if (filenamePos == string::npos)
        return make_error_body(BAD_REQUEST);
    filenamePos += 10; // Move past 'filename="'
    size_t filenameEndPos = bodyHeaders.find("\"", filenamePos);
    if (filenameEndPos == string::npos)
        return make_error_body(BAD_REQUEST);
    string fileName = bodyHeaders.substr(filenamePos, filenameEndPos - filenamePos);
    fileName        = dirPath + "/" + fileName;

    // Ensure the upload directory exists
    struct stat st;
    if (stat(dirPath.c_str(), &st) != 0) {
        if (mkdir(dirPath.c_str(), 0700) != 0)
            return make_error_body(INTERNAL_SERVER_ERROR);
    }
    else if (!S_ISDIR(st.st_mode)) {
        return make_error_body(INTERNAL_SERVER_ERROR);
    }

    ofstream outputFile(fileName.c_str(), std::ios_base::binary);
    if (outputFile.fail())
        return make_error_body(INTERNAL_SERVER_ERROR);

    outputFile.write(fileContent.c_str(), fileContent.size());
    outputFile.close();

    status = CREATED;
    return vector<char>();
}
