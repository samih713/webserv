#include "error_pages.hpp"

/**
 * @brief Creates a map of status codes and their corresponding string representations.
 *
 * This function creates a map where the keys are status codes and the values are their
 * string representations. The status codes are defined in the STATUS_CODE_LIST macro.
 *
 * @return A StatusCodeMap object containing the status codes and their string
 * representations.
 */
static StatusCodeMap createStatusCodeMap()
{
    StatusCodeMap map;
#define X(a) map.insert(make_pair(a, #a));
    STATUS_CODE_LIST
#undef X
    return map;
}

const StatusCodeMap status_codes_map = createStatusCodeMap();

static string generate_error_page_template(STATUS_CODE status)
{
	stringstream ss;

	// TODO css file should be loaded too
	ss << "<!DOCTYPE html>"
		<< "<html lang=\"en\">"
		<< "<head>"
		<< "    <meta charset=\"UTF-8\">"
		<< "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
		<< "    <title>"  << status << " " << status_codes_map.at(status) << "</title>"
		<< "    <link rel=\"stylesheet\" href=\"404style.css\">"
		<< "</head>"
		<< "<body>"
		<< "    <div class=\"container\">"
		<< "        <h1>" << status << " " << status_codes_map.at(status) << "</h1>"
		<< "        <a href=\"/\">Return Home</a>"
		<< "    </div>"
		<< "</body>"
		<< "</html>";

	return ss.str();
}

Page get_error_page(STATUS_CODE status, CachedPages& cpages, ServerConfig& config)
{
	Page page;
	(void)cpages; // Unused parameter (for now)

	DEBUGASSERT(status_codes_map.find(status) != status_codes_map.end());
    if (config.errorPages.find(status) == config.errorPages.end()) {
		// If the error page is not found, generate a default error page
		string errorPage = generate_error_page_template(status);
		page.data = vector<char>(errorPage.begin(), errorPage.end());
		page.contentLength = page.data.size();
	}
	else {
		// If the error page is found, load it from the cache
		ifstream error_page(config.errorPages.at(status).c_str(), std::ios_base::binary);
		if (error_page.fail())
			DEBUG_MSG("Error page failed to load", B);
		else {
			error_page.seekg(0, std::ios_base::end);
			page.contentLength = error_page.tellg();
			error_page.seekg(0, std::ios_base::beg);
			page.data = vector<char>((std::istreambuf_iterator<char>(error_page)),
										std::istreambuf_iterator<char>());
		}
	}
	page.contentType = "text/html";
    return page;
}
