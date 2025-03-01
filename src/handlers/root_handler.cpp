
#include "handlers/root_handler.h"

using namespace StructBX::Handlers;

RootHandler::RootHandler() :
    current_function_()
{
    requested_route_ = std::make_shared<Tools::Route>(std::vector<std::string>{""});
    set_security_type(Security::SecurityType::kDisableAll);
}

RootHandler::~RootHandler()
{

}

void RootHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    try
    {
        // Set request and response
            SetupRequest_(request);
            SetupResponse_(response);

            Tools::OutputLogger::Debug_(request.getURI() + " " + request.getMethod());
            
            if(!get_http_server_request().has_value())
            {
                JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "HTTP Request is Null.");
                return;
            }
            if(!get_http_server_response().has_value())
            {
                JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "HTTP Response is Null.");
                return;
            }

        // Add Server header
            AddHeader_("server", "structbx/" + std::string(PACKAGE_VERSION_COMPLETE));

        // Set requested route
            std::vector<std::string> segments;
            URI(request.getURI()).getPathSegments(segments);
            requested_route_ = std::make_shared<Tools::Route>(segments);

        // Setup Properties
            SetupProperties_();

        // Handler Process
            Process_();
    }
    catch(MySQL::MySQLException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error with the database or query. " + error.displayText());
    }
    catch(RuntimeException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + error.displayText());
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + error.displayText());
    }
    catch(std::out_of_range& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): " + std::string(error.what()));
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. " + std::string(error.what()));
    }
    catch(...)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on handleRequest(): No handled exception.");
        JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Internal server error. No handled exception." );
    }
}

bool RootHandler::SetupSSL_()
{
    auto request = get_http_server_request().value();
    SecureStreamSocket socket = static_cast<Net::HTTPServerRequestImpl&>(*request).socket();
    if (socket.havePeerCertificate())
        X509Certificate cert = socket.peerCertificate();

    return true;
}

StructBX::Functions::Function::Ptr RootHandler::AddFunction_(std::string endpoint, HTTP::EnumMethods method)
{
    return functions_manager_.AddFunction_(endpoint, method);
}

bool RootHandler::VerifySession_()
{
    // Extract session ID
        auto request = get_http_server_request().value();
        std::string session_id;
        Net::NameValueCollection cookies;
        request->getCookies(cookies);
        auto cookie_session = cookies.find("structbx-sid");
        auto sessions = Sessions::SessionsManager::get_sessions();

    // Verify Cookie session and session
        if(cookie_session != cookies.end())
        {
            session_id = cookie_session->second;
            if(sessions.find(session_id) == sessions.end())
            {
                return false;
            }

            // Get the session user
                auto id_user = sessions.at(session_id).get_id_user();
                get_users_manager().get_current_user().set_id(id_user);

            return true;
        }
        else
            return false;
}

bool RootHandler::VerifyPermissions_()
{
    return VerifyRoutesPermissions_(*requested_route_, properties_.method);
}

bool RootHandler::IdentifyRoute_()
{
    for(auto& it : get_functions_manager().get_functions())
    {
        if(requested_route_->SegmentsToString_() == it.first)
        {
            // Verify same HTTP Method
            if(GetMethod_(properties_.method) != it.second->get_method())
                continue;

            // Copy function and reset actions results
            current_function_ = it.second;
            for(auto& action : current_function_->get_actions())
                action->get_results() = std::make_shared<Query::Results>();

            return true;
        }
    }

    return false;
}

void RootHandler::ManageRequestBody_()
{
    auto request = get_http_server_request().value();
    if(properties_.method == "POST" || properties_.method == "PUT")
    {
        if(properties_.content_type == "multipart/form-data")
        {
            ReadFormMultipart_(*request);
        }
        else if(properties_.content_type == "application/x-www-form-urlencoded")
        {
            ReadFormURLEncoded_(*request, request->stream());
        }
        else if(properties_.content_type == "application/json")
        {
            ReadJSON_(request->stream());
        }
    }
    else
    {
        std::string uri = request->getURI();
        ReadFromURI_(uri);
    }
}

void RootHandler::SetupProperties_()
{
    auto request = get_http_server_request().value();
    if(!get_http_server_request().has_value())
        return;

    properties_.method = IdentifyMethod_(request->getMethod());
    properties_.uri = request->getURI();
    properties_.content_type = request->getContentType();
    Net::MessageHeader::splitParameters(request->getContentType(), properties_.content_type, properties_.content_type_parameters);
}

void RootHandler::IdentifyParameters_()
{
    if(current_function_ == nullptr)
    {
        auto function = AddFunction_("", HTTP::EnumMethods::kHTTP_GET);
        current_function_ = function;
    }

    switch(get_body_type())
    {
        case HTTP::Body::Type::kFormMultipart:
            IdentifyParameters_(get_form());
            IdentifyParameters_(*get_files_parameters());
            break;
        case HTTP::Body::Type::kJSON:
            IdentifyParameters_(get_json_array());
            break;
        case HTTP::Body::Type::kURI:
        case HTTP::Body::Type::kFormURLEncoded:
            IdentifyParameters_(get_query_parameters());
            if(get_json_array()->size() > 0)
                IdentifyParameters_(get_json_array());
            break;
    }

    auto& actions = current_function_->get_actions();
    for(auto it = actions.begin(); it != actions.end(); ++it)
    {
        current_function_->IdentifyParameters_(*it);
    }
}

void RootHandler::IdentifyParameters_(JSON::Array::Ptr json_array)
{
    try
    {
        // Iterate over JSON array
        for (std::size_t a = 0; a < json_array->size(); a++)
        {
            // Get Parameter object
            auto parameter_object = json_array->getObject(a);
            if(parameter_object == nullptr)
            {
                Tools::OutputLogger::Warning_("Warning on root_handler.cpp on IdentifyParameters_(): Parameter JSON object is null.");
                continue;
            }

            // Get parameter name
            if(parameter_object->get("name").isEmpty())
            {
                Tools::OutputLogger::Warning_("Warning on root_handler.cpp on IdentifyParameters_(): Parameter name is empty.");
                continue;
            }
            auto parameter_name = parameter_object->get("name").toString();
                
            // Get parameter value
            if(parameter_object->get("value").isEmpty())
            {
                Tools::OutputLogger::Warning_("Warning on root_handler.cpp on IdentifyParameters_(): Parameter value is empty.");
                continue;
            }
            auto parameter_value = parameter_object->get("value").toString();
            
            // Create and Save parameter
            auto parameter = std::make_shared<Query::Parameter>(parameter_name, Tools::DValue::Ptr(new Tools::DValue(parameter_value)), true);

            current_function_->get_parameters().push_back(parameter);
        }
    }
    catch(JSON::JSONException& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
}

void RootHandler::IdentifyParameters_(std::shared_ptr<Net::HTMLForm> form)
{
    try
    {
        // Iterate over files
        for (auto& value : *form)
        {
            // Get parameter object
            Query::Parameter::Ptr parameter(new Query::Parameter(value.first, Tools::DValue::Ptr(new Tools::DValue(value.second)), true));
            current_function_->get_parameters().push_back(parameter);
        }
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
}

void RootHandler::IdentifyParameters_(Files::FileManager& files_parameters)
{
    try
    {
        // Iterate over files
        for (auto& file : files_parameters.get_files())
        {
            // Verify file size
            float filesize = file.get_tmp_file()->getSize();
            if(filesize > Tools::SettingsManager::GetSetting_("max_file_size", 15) * 1000000)
            {
                Tools::OutputLogger::Warning_("Warning on root_handler.cpp on IdentifyParameters_(): The file " + file.get_name()
                    + " exceeds the maximum file size (" + std::to_string(Tools::SettingsManager::GetSetting_("max_file_size", 15)) + ")");
                continue;
            }

            // Create parameter
            auto parameter = std::make_shared<Query::Parameter>(file.get_name(), Tools::DValue::Ptr(new Tools::DValue()), true);

            // Copy the file to parameter value
            std::ifstream istr; std::stringstream parameter_value;
            istr.open(file.get_tmp_file()->path());
            StreamCopier::copyStream(istr, parameter_value);
            istr.close();

            auto value = std::make_shared<Tools::DValue>(parameter_value.str());
            parameter->set_value(value);

            current_function_->get_parameters().push_back(parameter);
        }
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
}

void RootHandler::IdentifyParameters_(URI::QueryParameters& query_parameters)
{
    try
    {
        // Iterate over JSON array
        for (auto& query_parameter : query_parameters)
        {
            // Get parameter object
            auto parameter = std::make_shared<Query::Parameter>(query_parameter.first, Tools::DValue::Ptr(new Tools::DValue(query_parameter.second)), true);
            current_function_->get_parameters().push_back(parameter);
        }
    }
    catch(std::runtime_error& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
    catch(std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on root_handler.cpp on IdentifyParameters_(): " + std::string(error.what()));
        return;
    }
}
