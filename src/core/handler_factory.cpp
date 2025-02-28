
#include "handler_factory.h"

using namespace StructBX::Core;

HandlerFactory::HandlerFactory() :
    handler_creator_(nullptr)
{

}

HandlerFactory::~HandlerFactory()
{

}

HTTPRequestHandler* HandlerFactory::createRequestHandler(const HTTPServerRequest& request)
{
    try
    {
        if(handler_creator_ == nullptr)
        {
            handler_creator_ = [&](Core::HTTPRequestInfo& info)
            {
                // Find route and handler
                FunctionHandler f;

                auto found = connections_.find(info.uri);
                if(found != connections_.end())
                    f = found->second.return_handler_;
                else
                    f = [&](){return new StructBX::Handlers::NullHandler();};

                return f();
            };
        }
        else
        {
            Core::HTTPRequestInfo info(request.getURI(), request.getMethod());
            return handler_creator_(info);
        }
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Error_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        ErrorResponse_(request, "Error with the database. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Error_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        ErrorResponse_(request, "Internal server error. " + error.displayText());
    }
    catch(Poco::NullPointerException& error)
    {
        Tools::OutputLogger::Error_("Error on handler_factory.cpp on createRequestHandler(): " + error.displayText());
        ErrorResponse_(request, "Internal server error. " + error.displayText());
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        ErrorResponse_(request, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on handler_factory.cpp on createRequestHandler(): " + std::string(error.what()));
        ErrorResponse_(request, "Internal server error. " +  std::string(error.what()));
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