#include "DeleteRequestHandler.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"
#include "PostRequestHandler.hpp"
#include <algorithm>

#ifndef REQUESTHANDLERFACTORY_HPP
#define REQUESTHANDLERFACTORY_HPP

IRequestHandler* MakeRequestHandler(const string& method)
{
    if (method == "GET")
        return new GetRequestHandler;
    else if (method == "POST")
        return new PostRequestHandler;
    else if (method == "DELETE")
        return new DeleteRequestHandler;
    else  //! return 501 Not implemented
        THROW_EXCEPTION_WITH_INFO("Request Method not implemented\n");
}

#endif // REQUESTHANDLERFACTORY_HPP
