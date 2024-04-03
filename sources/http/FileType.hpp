#include "../../includes/webserv.hpp"

#ifndef FILETYPE_HPP
#define FILETYPE_HPP

// typedef char supported file extensions
typedef map<string, string> types_map_t;

static const std::pair<std::string, std::string> init_filetypes[] = {
    std::make_pair("html", "text/html"),
    std::make_pair("css", "text/css"),
    std::make_pair("js", "application/javascript"),
    std::make_pair("json", "application/json"),
    std::make_pair("png", "image/png"),
    std::make_pair("jpg", "image/jpeg"),
    std::make_pair("jpeg", "image/jpeg"),
    std::make_pair("gif", "image/gif"),
    std::make_pair("svg", "image/svg+xml"),
    std::make_pair("pdf", "application/pdf"),
    std::make_pair("xml", "application/xml"),
    std::make_pair("txt", "text/plain"),
    std::make_pair("mp4", "video/mp4"),
    std::make_pair("mp3", "audio/mpeg"),
    std::make_pair("wav", "audio/wav"),
    std::make_pair("webp", "image/webp"),
    std::make_pair("ico", "image/x-icon")
};

static const map<string, string> fileTypes(init_filetypes,
                                           init_filetypes +
                                               sizeof(init_filetypes) /
                                                   sizeof(init_filetypes[0]));

#endif // FILETYPE_HPP
