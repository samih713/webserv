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

            // separating resource and query string
            size_t queryPos = header.resource.find('?');
            if (queryPos != string::npos) {
                queryString     = header.resource.substr(queryPos + 1);
                header.resource = header.resource.substr(0, queryPos);
            }
        }
        if (header(PARSED) && parse_body())
            return true;
    } catch (std::ios_base::failure& f) {
        set_status(BAD_REQUEST);
        DEBUG_MSG(ERR_PARSE + ": " + string(f.what()), R);
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
    // check if the resource path matches a location
    string locationMatch;
    for (map<string, Location>::const_iterator location = config.locations.begin();
         location != config.locations.end(); ++location)
    {
        if (header.resource.find(location->first) == 0) {
            if (location->first.size() > locationMatch.size())
                locationMatch = location->first;
        }
    }

    // update resource path and body size
    if (locationMatch.empty())
        header.resource = config.root + "/" + header.resource;
    else {
        const Location& location = config.locations.at(locationMatch);
        header.resource =
            location.root + "/" + header.resource.substr(locationMatch.size());
    }
    header.bodySize = std::min(header.bodySize, config.maxBodySize);
}
