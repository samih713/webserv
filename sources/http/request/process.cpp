#include "Logger.hpp"
#include "Message.hpp"
#include "Request.hpp"
#include <ios>

/**
 * @brief Process the HTTP request by parsing the request
 * header and body. It applies the provided server configuration to the request header and
 * checks if the request body can be parsed successfully.
 *
 * @param cfg The server configuration to apply to the request header.
 * @return True if the request was processed successfully, false otherwise.
 */
bool Request::process(const ServerConfig& cfg)
{
    try {
        if (header(READY_TO_PARSE)) {
            // setup stream to throw on parsing errors
            message.exceptions(std::ios::failbit | std::ios::badbit);
            header.parse_header(message);
            header.state = PARSED;
            apply_config(cfg);
        }
        if (header(PARSED) && parse_body())
            return true;
    } catch (std::ios_base::failure& f) {
        set_status(BAD_REQUEST);
        LOG_ERROR(ERR_PARSE + ": " + string(f.what()));
        return true;
    }
    return false;
}

/**
 * @brief Modifies the request header by updating the resource paths and body size based
 * on the server configuration.
 *
 * @param cfg The server configuration to apply.
 */
void Request::apply_config(const ServerConfig& cfg)
{
    // check if the resource path matches a location
    for (map<string, Location>::const_iterator location = cfg.locations.begin();
         location != cfg.locations.end(); ++location)
    {
        if (header.resource.find(location->first) == 0) {
            if (location->first.size() > locationMatch.size())
                locationMatch = location->first;
        }
    }

    // update resource path and body size
    string& uri         = header.resource;
    string  root        = cfg.root;
    size_t  maxBodySize = cfg.maxBodySize;
    if (!locationMatch.empty()) {
        const Location& location = cfg.locations.at(locationMatch);
        root                     = location.root;
        uri                      = header.resource.substr(locationMatch.size());
        maxBodySize              = location.maxBodySize;
    }
    uri          = (uri[0] != '/') ? ("/" + uri) : uri;
    resourcePath = root + uri;

    if (header.bodySize > maxBodySize)
        set_status(PAYLOAD_TOO_LARGE);
    header.bodySize = std::min(header.bodySize, maxBodySize);
}
