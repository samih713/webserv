#include "webserv.hpp"

#ifndef FILETYPE_HPP
#define FILETYPE_HPP

static const pair<string, string> typeList[] = {
    make_pair("html", "text/html"),
    make_pair("css", "text/css"),
    make_pair("js", "application/javascript"),
    make_pair("json", "application/json"),
    make_pair("png", "image/png"),
    make_pair("jpg", "image/jpeg"),
    make_pair("jpeg", "image/jpeg"),
    make_pair("gif", "image/gif"),
    make_pair("svg", "image/svg+xml"),
    make_pair("pdf", "application/pdf"),
    make_pair("xml", "application/xml"),
    make_pair("txt", "text/plain"),
    make_pair("mp4", "video/mp4"),
    make_pair("mp3", "audio/mpeg"),
    make_pair("wav", "audio/wav"),
    make_pair("webp", "image/webp"),
    make_pair("ico", "image/x-icon"),
    make_pair("sh", "bash"),
    make_pair("py", "python"),
};

static const HeaderMap fileTypes(typeList,
    typeList + sizeof(typeList) / sizeof(typeList[0]));

#endif // FILETYPE_HPP
