
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

using namespace StructBI;
using namespace NAF;

void SetupSettings()
{
    NAF::Tools::SettingsManager::AddSetting_("directory_for_uploaded_files", NAF::Tools::DValue::Type::kString, NAF::Tools::DValue("/var/www/structbi-web-uploaded"));
}

int main(int argc, char** argv)
{
    // Setup
        Core::NebulaAtom app(true);

        NAF::Tools::SettingsManager::ReadSettings_();
        app.SetupSettings_();
        SetupSettings();
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
                    handler = new StructBI::Webserver;
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
                        handler = new StructBI::BackendServer;

                    break;
                }

                return handler;
            }

            return handler;
        });

    // Run
        auto code = app.Init_(argc, argv);

    // End
        NAF::Query::DatabaseManager::StopMySQL_();
        return code;
}
