#include <algorithm>
#include "FindHeaderKey.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"

#ifndef REQUESTHANDLERFACTORY_HPP
#define REQUESTHANDLERFACTORY_HPP

/**
 * Creates a new instance of a request handler based on the specified HTTP method.
 *
 * @param m The HTTP method for which to create a request handler.
 * @return A pointer to the newly created request handler.
 * @throws std::runtime_error if the specified HTTP method is not implemented.
 */
class RequestHandlerFactory {
public:
    static IRequestHandler* MakeRequestHandler(string method)
    {
        METHOD m = find_method(method);
        switch (m) {
            case GET: return new GetRequestHandler;
            default:  THROW_EXCEPTION_WITH_INFO("Request Method not implemented\n");
        }
    }

private:
    enum METHOD {
        GET = 0,
        POST,
        PUT,
        DELETE,
        HEAD,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    };
    static METHOD find_method(string method)
    {
        vector<string> methods;
        init_methods(methods);
        vector<string>::iterator it = std::find(methods.begin(), methods.end(), method);
        return static_cast<METHOD>(std::distance(methods.begin(), it));
    }
    static void init_methods(vector<string> &methods);
    
};

void RequestHandlerFactory::init_methods(vector<string> &methods)
{
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("PUT");
    methods.push_back("DELETE");
    methods.push_back("HEAD");
    methods.push_back("CONNECT");
    methods.push_back("OPTIONS");
    methods.push_back("TRACE");
    methods.push_back("PATCH");
};
#endif // REQUESTHANDLERFACTORY_HPP
