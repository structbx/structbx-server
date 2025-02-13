
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include <query/parameter.h>
#include <tools/dvalue.h>
#include <tools/output_logger.h>
#include <tools/hmac_tool.h>

#include "web_server.h"
#include "backend_server.h"

using namespace StructBX;
using namespace NAF;

struct Parameters
{
    std::string properties_file = "";
};

void SetupSettings()
{
    NAF::Tools::SettingsManager::AddSetting_("directory_for_uploaded_files", NAF::Tools::DValue::Type::kString, NAF::Tools::DValue("/var/www/structbx-web-uploaded"));
    NAF::Tools::SettingsManager::AddSetting_("space_id_cookie_name", NAF::Tools::DValue::Type::kString, NAF::Tools::DValue("1f3efd18688d2"));
}

Parameters SetupParameters(std::vector<std::string>& parameters)
{
    Parameters params;

    // Properties file
    auto config = std::find(parameters.begin(), parameters.end(), "--config");
    params.properties_file = config != parameters.end() ? *(config + 1) : "properties.yaml";

    // Remove all parameters except the first one
    std::string first_param = parameters.front();
    parameters = std::vector<std::string>();
    parameters.push_back(first_param);

    return params;
}

int main(int argc, char** argv)
{
    // Setup
        Core::NebulaAtom app(true);

    // Parameters
        auto& parameters = app.get_console_parameters();
        parameters = std::vector<std::string>(argv, argv + argc);
        Parameters params = SetupParameters(parameters);

    // Settings
        NAF::Tools::SettingsManager::set_config_path(params.properties_file);
        NAF::Tools::OutputLogger::set_log_to_file(true);
        SetupSettings();
        NAF::Tools::SettingsManager::ReadSettings_();
        app.SetupSettings_();

    // Setup
        NAF::Query::DatabaseManager::StartMySQL_();
        NAF::Security::PermissionsManager::LoadPermissions_();
        NAF::Tools::SessionsManager::ReadSessions_();

    // Custom Handler Creator
        app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
        {
            NAF::Handlers::RootHandler* handler;

            // Manage the route type
            NAF::Tools::Route route(info.uri);

            switch(route.get_current_route_type())
            {
                // Manage Frontend
                case NAF::Tools::RouteType::kEntrypoint:
                {
                    handler = new StructBX::Webserver;
                    break;
                }

                // Manage Backend
                case NAF::Tools::RouteType::kEndpoint:
                {
                    // Routes
                    NAF::Tools::Route requested_route(info.uri);
                    NAF::Tools::Route login_route("/api/system/login");
                    NAF::Tools::Route logout_route("/api/system/logout");

                    if(requested_route == login_route || requested_route == logout_route)
                    {
                        handler = new NAF::Handlers::LoginHandler();
                        auto password = handler->get_users_manager().get_action()->GetParameter("password");
                        if(password != handler->get_users_manager().get_action()->get_parameters().end())
                        {
                            password->get()->SetupCondition_("condition-password", Query::ConditionType::kWarning, [](Query::Parameter::Ptr param)
                            {
                                std::string password = param->ToString_();
                                std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
                                param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
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
        });

    // Run
        auto code = app.Init_();

    // End
        NAF::Query::DatabaseManager::StopMySQL_();
        return code;
}
