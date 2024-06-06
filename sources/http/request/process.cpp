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
    string locationMatch;
    for (map<string, Location>::const_iterator location = cfg.locations.begin();
         location != cfg.locations.end(); ++location)
    {
        if (header.resource.find(location->first) == 0) {
            if (location->first.size() > locationMatch.size())
                locationMatch = location->first;
        }
    }

    // update resource path and body size
    if (locationMatch.empty())
        header.resource = cfg.root + "/" + header.resource;
    else {
        const string& locationRoot = cfg.locations.at(locationMatch).root;
        const string& uri          = header.resource.substr(locationMatch.size());
        if (uri[0] == '/')
            header.resource = locationRoot + uri;
        else
            header.resource = locationRoot + "/" + uri;
    }
    header.bodySize = std::min(header.bodySize, cfg.maxBodySize);
}
