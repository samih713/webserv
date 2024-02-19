#include "ResponseBuilder.hpp"
#include "Message.hpp"
#include "debug.hpp"
#include "webserv.hpp"
#include <algorithm>
#include <utility>

ResponseBuilder::ResponseBuilder(const Request &request)
    : request(request)
{
    DEBUG_MSG("ResponseBuilder called\n", R);
}

ResponseBuilder::~ResponseBuilder()
{}

ResponseBuilder::ResponseBuilder(const ResponseBuilder &other)
    : request(other.request)
{}

ResponseBuilder &ResponseBuilder::operator=(const ResponseBuilder &)
{
    return *this;
}

void ResponseBuilder::set_status_code()
{}

struct FindKey
{
        FindKey(const string &key)
            : key(key){};
        bool operator()(const pair<string, string> &header)
        {
            return header.first == key;
        }

    private:
        const string &key;
};


STATUS_CODE ResponseBuilder::build_body() const
{
    string resource = request.get_resource();
    METHOD method = request.get_method();
    vsp    headers = request.get_headers();
    string path = "";
    switch (method)
    {
        case GET:
        {
            if (resource[0] == '/')
            {
                FindKey             a("Host");
                vsp::const_iterator it = std::find_if(headers.begin(), headers.end(), a);
                if (it != headers.end())
                {
                    // cout << it->second << endl;
                    // cout << resource << endl;
                    path = it->second + resource;
                    // cout << it->first << it->second << endl;
                    cout << path << endl;
                }

                // for (size_t i = 0; i < headers.size(); i++)
                // {
                //     if (headers[i].first == "Host")
                //         cout << headers[i].second << endl;
                // }
            }
            break;
        }
        default: break;
    }
    return OK;
}

const Response ResponseBuilder::get_response()
{
    vsp         headers;
    string      body;
    STATUS_CODE status = build_body();


    Response response(status, headers, body);

    return response;
}
