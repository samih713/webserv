#include "Message.hpp"
#include "Request.hpp"
#include "webserv.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

//! this is needed because in a case where the request is not found, the server should
//! return a 404 error page. should be removed and combined with the CachedPages.hpp
static const string ERR_SEND_FAIL("Response: Failed to send: ");

class Response {
public:
    Response(STATUS_CODE status, const HeaderMap& headers, const vector<char>& body);
    ~Response();
    Response(const Response& other);
    void send_response(fd recv_socket) const;

private:
    STATUS_CODE  status;
    HeaderMap    headers;
    vector<char> body;
    inline void  load_status_line(ostringstream& os) const;
    inline void  load_headers(ostringstream& os) const;
    // deleted copy assignment
    void operator=(const Response&);
};

#endif // RESPONSE_HPP
