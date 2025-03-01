
#include "handler_factory.h"

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
                handler = new StructBX::Webserver;
                break;
            }

            // Manage Backend
            case StructBX::Tools::RouteType::kEndpoint:
            {
                // Routes
                StructBX::Tools::Route requested_route(request.getURI());
                StructBX::Tools::Route login_route("/api/system/login");
                StructBX::Tools::Route logout_route("/api/system/logout");

                if(requested_route == login_route || requested_route == logout_route)
                {
                    handler = new StructBX::Handlers::LoginHandler();
                    auto password = handler->get_users_manager().get_action()->GetParameter("password");
                    if(password != handler->get_users_manager().get_action()->get_parameters().end())
                    {
                        password->get()->SetupCondition_("condition-password", Query::ConditionType::kWarning, [](Query::Parameter::Ptr param)
                        {
                            std::string password = param->ToString_();
                            std::string password_encoded = StructBX::Tools::HMACTool().Encode_(password);
                            param->set_value(StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(password_encoded)));
                            return true;
                        });
                    }
                }
                else
                    handler = new StructBX::BackendServer;

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

    out << "<html>";
    out << "<head><title> 500 HTTP_INTERNAL_SERVER_ERROR | Nebula Atom</title></head>";
    out << "<body>";
    out << "<center><h1>Status: 500 HTTP_INTERNAL_SERVER_ERROR </h1></center>";
    out << "<center><h3>Message: " << error << "</h3></center>";
    out << "<center><hr>Nebula Atom/" << PACKAGE_VERSION_COMPLETE << "</center>";
    out << "</body>";
    out << "</html>";
}