#include "Message.hpp"
#include "Request.hpp"
#include <ios>


/**
 * @brief Process the HTTP request by parsing the request
 * header and body. It applies the provided server configuration to the request header and
 * checks if the request body can be parsed successfully.
 *
 * @param config The server configuration to apply to the request header.
 * @return True if the request was processed successfully, false otherwise.
 */
bool Request::process(const ServerConfig& config)
{
    try {
        if (header(READY_TO_PARSE)) {
            // setup stream to throw on parsing errors
            message.exceptions(std::ios::failbit | std::ios::badbit);
            header.parse_header(message);
            header.state = PARSED;
            apply_config(config);
        }
        if (header(PARSED) && parse_body()) {
            // DEBUG_MSG(">>>>>Request processed successfully<<<<<", R);
            // DEBUG_MSG(*this, R);
            // DEBUG_MSG(">>>>>------------------------------<<<<<", R);
            return true;
        }
    } catch (std::ios_base::failure& f) {
        set_status(BAD_REQUEST);
        DEBUG_MSG(ERR_PARSE + string(f.what()), R);
        return true;
    }
    return false;
}

/**
 * @brief Modifies the request header by updating the resource paths and body size based
 * on the server configuration.
 *
 * @param config The server configuration to apply.
 */
void Request::apply_config(const ServerConfig& config)
{
    header.resource    = config.serverRoot + header.resource;
    header.cgiResource = config.serverRoot + header.resource; // change to cgi root
    header.bodySize    = std::min(header.bodySize, config.maxBodySize);
}
