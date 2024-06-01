#include "CachedPages.hpp"
#include "Logger.hpp"

typedef map<STATUS_CODE, string> StatusCodeMap;

// ! combine it please
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

// loads all the error pages found in the config,
// if the loading fails then generate the error page and add it to the map
CachedPages::CachedPages(const ServerConfig& cfg)
{
    //  load the error pages in config
    for (map<STATUS_CODE, string>::const_iterator it = cfg.errorPages.begin();
         it != cfg.errorPages.end(); it++)
    {
        if (!load_page(it->second, ws_itoa(it->first))) {
            LOG_INFO("Generating error page " + ws_itoa(it->first));
            generate_error_page(it->first);
        }
    }
    // load the indexDefault page
    if (!load_page(cfg.indexFile, "index"))
        LOG_ERROR("Failed to load index page " + cfg.indexFile);
}

bool CachedPages::load_page(const string& path, const string& name)
{
    ifstream pFile(path.c_str(), std::ios_base::binary);
    if (pFile.fail()) {
        LOG_ERROR("Failed to load error page " + path);
        return false;
    }

    Page page;
    page.data          = vector<char>((std::istreambuf_iterator<char>(pFile)),
                 std::istreambuf_iterator<char>());
    page.contentLength = page.data.size();
    pages.insert(make_pair(name, page));

    return true;
}

static string generate_error_page_template(STATUS_CODE status)
{
    stringstream ss;

    // clang-format off
    // TODO css file should be inlined
    ss << "<!DOCTYPE html>"
       << "<html lang=\"en\">"
       << "<head>"
       << "    <meta charset=\"UTF-8\">"
       << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
       << "    <title>" << status << " " << status_codes_map.at(status) << "</title>"
       << "    <link rel=\"stylesheet\" href=\"style.css\">"
       << "</head>"
       << "<body>"
       << "    <div class=\"container\">"
       << "        <h1>" << status << " " << status_codes_map.at(status) << "</h1>"
       << "        <a href=\"/\">Return Home</a>"
       << "    </div>"
       << "</body>"
       << "</html>";
    // clang-format on

    return ss.str();
}

//! should throw incase it wasn't able to for some reason
void CachedPages::generate_error_page(STATUS_CODE status)
{
    Page   page;
    string errorPage   = generate_error_page_template(status);
    page.data          = vector<char>(errorPage.begin(), errorPage.end());
    page.contentLength = page.data.size();
    pages.insert(make_pair(ws_itoa(status), page));
}

Page& CachedPages::get_error_page(STATUS_CODE status)
{
    if (pages.find(ws_itoa(status)) == pages.end()) {
        LOG_INFO("Generating error page " + ws_itoa(status));
        generate_error_page(status);
    }
    return pages.at(ws_itoa(status));
}

Page& CachedPages::get_page(const string& pageName)
{
    if (pages.find(pageName) == pages.end()) {
        LOG_ERROR("Page " + pageName + " not found");
        // return 404 page
        return get_error_page(NOT_FOUND);
    }
    return pages.at(pageName);
}