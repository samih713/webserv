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
    DEBUG_MSG("Handling POST", W);
    (void) cachedPages;
    (void) config;
    (void) request;

    vector<char> body('a');
    Response     response(NOT_FOUND, response_headers, body);
    return response;
}
