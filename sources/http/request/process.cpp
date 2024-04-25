#include "Message.hpp"
#include "Request.hpp"

// processing the request is basically 2 parts
//	[1] - header
//	[2] - body

// throws ios failure
bool Request::process(const ServerConfig& config)
{
    // expectedBodySize = std::min(expectedBodySize, maxBodySize);

    // setup stream to throw on parsing errors
    message.exceptions(std::ios::failbit | std::ios::badbit);

    if (header(READY_TO_PARSE))
        header.parse_header(message);
    else
        return false;

    if (parse_body())
        apply_config(config);
    else
        return false;

    return true;
    // if (expectedBodySize == NOT_SPECIFIED)
    //     return true;
}

// applies the configuration parameters to the request
void Request::apply_config(const ServerConfig& config)
{
    header.resource    = config.serverRoot + header.resource;
    header.cgiResource = config.serverRoot + header.resource; // change to cgi root
    header.bodySize    = std::min(header.bodySize, config.maxBodySize);
}
