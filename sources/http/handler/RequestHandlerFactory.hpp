#include "../FindHeaderKey.hpp"
#include "./GetRequestHandler.hpp"
#include "./IRequestHandler.hpp"

#ifndef REQUESTHANDLERFACTORY_HPP
#define REQUESTHANDLERFACTORY_HPP

/**
 * Creates a new instance of a request handler based on the specified HTTP method.
 *
 * @param m The HTTP method for which to create a request handler.
 * @return A pointer to the newly created request handler.
 * @throws std::runtime_error if the specified HTTP method is not implemented.
 */
class RequestHandlerFactory
{
    public:
        static IRequestHandler *MakeRequestHandler(METHOD m)
        {
            switch (m)
            {
                case GET: return new GetRequestHandler;
                default: THROW_EXCEPTION_WITH_INFO("Request Method not implemented\n");
            }
        }
};

#endif // REQUESTHANDLERFACTORY_HPP
