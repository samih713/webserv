#include "../../../includes/webserv.hpp"
#include "../Message.hpp"
#include "../request/Request.hpp"

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

typedef std::map<STATUS_CODE, std::string> StatusCodeMap;

static const string ERR_SEND_FAIL("Response: Failed to send: ");

class Response {
public:
    Response(STATUS_CODE status, const vsp& headers, const vector<char>& body);
    ~Response();
    Response(const Response& other);
    void send_response(fd recv_socket) const;

private:
    STATUS_CODE  status;
    vsp          headers;
    vector<char> body;
    inline void  load_status_line(ostringstream& os) const;
    inline void  load_headers(ostringstream& os) const;
    // deleted copy assignment
    void operator=(const Response&);
    // response constants

    static const string& version;
};

#endif // RESPONSE_HPP
