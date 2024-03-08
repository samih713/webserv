#include "FindHeaderKey.hpp"
#include "GetRequestHandler.hpp"
#include "IRequestHandler.hpp"

#ifndef REQUESTHANDLERFACTORY_HPP
#define REQUESTHANDLERFACTORY_HPP

class RequestHandlerFactory
{
    public:
        static IRequestHandler *MakeRequestHandler(METHOD m)
        {
            switch (m)
            {
                case GET: return new GetRequestHandler;
                default: throw std::runtime_error("Request Method not implemented\n");
            }
        }
};

#endif // REQUESTHANDLERFACTORY_HPP
