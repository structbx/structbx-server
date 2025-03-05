
#include "handler_factory.h"
#include "http/html_message.h"

using namespace StructBX::Core;

HandlerFactory::HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    try
    {
        StructBX::Handlers::RootHandler* handler;

        // Manage the route type
        StructBX::Tools::Route route(request.getURI());

        switch(route.get_current_route_type())
        {
            // Manage Frontend
            case StructBX::Tools::RouteType::kEntrypoint:
            {
                handler = new StructBX::Handlers::WebHandler;
                break;
            }

            // Manage Backend
            case StructBX::Tools::RouteType::kEndpoint:
            {
                // Routes
                StructBX::Tools::Route requested_route(request.getURI());

                if(requested_route == Handlers::AuthRoutes::login_route || requested_route == Handlers::AuthRoutes::logout_route)
                    handler = new StructBX::Handlers::LoginHandler();
                else
                    handler = new StructBX::Handlers::BackendHandler;

                break;
            }

            return handler;
        }

        return handler;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        ErrorResponse_(request, "Internal server error. " +  std::string(error.what()));
    }
    catch(...)
    {
        Tools::OutputLogger::Error_("Unexpected error on handler_factory.cpp on createRequestHandler()");
        ErrorResponse_(request, "Unexpected error on handler_factory.cpp on createRequestHandler()");
    }

    return new StructBX::Handlers::NullHandler();
}

void HandlerFactory::ErrorResponse_(const HTTPServerRequest& request, std::string error)
{
    request.response().setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
    request.response().setContentType("text/html");
    request.response().setChunkedTransferEncoding(true);

    std::ostream& out = request.response().send();

    out << HTTP::HTMLMessage("500", "HTTP_INTERNAL_SERVER_ERROR", error);
}