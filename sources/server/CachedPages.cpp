#include "CachedPages.hpp"
#include "Logger.hpp"

// ! combine it please

// loads all the error pages found in the config,
// if the loading fails then generate the error page and add it to the map
CachedPages::CachedPages(const ServerConfig& cfg)
{
    //  load the error pages in config
    for (StatusCodeMap::const_iterator it = cfg.errorPages.begin();
         it != cfg.errorPages.end(); it++)
    {
        if (!load_page(it->second, ws_itoa(it->first))) {
            LOG_ERROR("Failed to load error page: " + it->second);
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
    if (pFile.fail())
        return false;

    Page page;
    page.data = vector<char>((std::istreambuf_iterator<char>(pFile)),
        std::istreambuf_iterator<char>());

    page.contentLength = page.data.size();
    pages.insert(make_pair(name, page));

    LOG_INFO("Loaded " + name + " page at: " + path);
    return true;
}

static string generate_error_page_template(STATUS_CODE status)
{
    stringstream ss;

    // clang-format off
    ss << "<!DOCTYPE html>\n"
       << "<html lang=\"en\">\n"
       << "<head>\n"
       << "  <meta charset=\"UTF-8\">\n"
       << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
       << "  <title>" << status << " " << status_codes_map.at(status) << "</title>"
       << "  <style>\n"
       << "    body, html { height: 100%; margin: 0; font-family: Arial, sans-serif; "
       << "        background-color: #f0f0f0; display: flex; justify-content: center; "
       << "        align-items: center; text-align: center; }\n"
       << "    .container { max-width: 600px; padding: 20px; background-color: #fff; "
       << "        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); word-wrap: break-word; }\n"
       << "    h1 { font-size: 48px; color: #ff6347; margin: 0; padding: 0; }\n"
       << "    p { font-size: 18px; }\n"
       << "    a { display: inline-block; margin-top: 20px; padding: 10px 20px; "
       << "        background-color: #ff6347; color: #fff; text-decoration: none; "
       << "        border-radius: 5px; }\n"
       << "    a:hover { background-color: #e55347; }\n"
       << "  </style>\n"
       << "</head>\n"
       << "<body>\n"
       << "  <div class=\"container\">\n"
       << "    <h1>" << status << " " << status_codes_map.at(status) << "</h1>\n"
       << "    <a href=\"/\">Return Home</a>\n"
       << "  </div>\n"
       << "</body>\n"
       << "</html>\n";
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
        return get_error_page(NOT_FOUND);
    }
    return pages.at(pageName);
}
