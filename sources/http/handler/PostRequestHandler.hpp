#include "Request.hpp"
#include "RequestHandlerBase.hpp"
#include "webserv.hpp"

#ifndef POST_REQUEST_HANDLER_HPP
#define POST_REQUEST_HANDLER_HPP

class PostRequestHandler: public RequestHandlerBase {
public:
    PostRequestHandler(ServerConfig& cfg) : RequestHandlerBase(cfg) {}
    ~PostRequestHandler() {}

    Response handle_request(const Request& r);

private:
    vector<char> process_data(const Request& r);
    bool         send_request_body(const string& body);
    vector<char> handle_upload(const Request& r, const string& path);
};

#endif // POST_REQUEST_HANDLER_HPP
