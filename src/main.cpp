
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "handlers/frontend_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include <query/parameter.h>
#include <tools/dvalue.h>

using namespace Atom;

class Webserver : public Handlers::FrontendHandler
{
    public:
        Webserver()
        {
            // Set headers
            AddHeader_("Access-Control-Allow-Credentials", "true");
            AddHeader_("Access-Control-Allow-Headers", "Content-Type");
            AddHeader_("Access-Control-Allow-Methods", "*");
            AddHeader_("Access-Control-Allow-Origin", "https://localhost:3000");
        }
        virtual ~Webserver(){}

        void Process_()
        {
            SetupHeaders_();
            get_file_manager().AddBasicSupportedFiles_();
            DownloadProcess_();
        }
};

class MainBackendHandler : public Handlers::BackendHandler
{
    public:
        MainBackendHandler() : Handlers::BackendHandler(){}
        virtual ~MainBackendHandler() {}

        void AddFunctions_()
        {
            // Function GET /api/forms
                auto function = AddFunction_("/api/forms/read", HTTP::EnumMethods::kHTTP_GET);

                auto action = function->AddAction_("a1");
                action->set_sql_code("SELECT * FROM forms");
                action->set_final(true);

            // Function POST /api/forms
                auto function2 = AddFunction_("/api/forms/add", HTTP::EnumMethods::kHTTP_POST);

                auto action2 = function2->AddAction_("a1");
                action2->set_sql_code("INSERT INTO forms (identifier, name, state, description, id_cloud_organization) VALUES (?, ?, ?, ?, 1)");
                action2->set_final(true);

                // Parameters and conditions
                action2->AddParameter_("identifier", Tools::DValue(""), true)
                ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                {
                    if(param->get_value().get_type() != Tools::DValue::Type::kString)
                    {
                        param->set_error("El identificador debe ser una cadena de texto");
                        return false;
                    }
                    if(param->get_value().ToString_() == "")
                    {
                        param->set_error("El identificador no puede estar vacío");
                        return false;
                    }
                    if(param->get_value().ToString_().size() < 3)
                    {
                        param->set_error("El identificador no puede ser menor a 3 dígitos");
                        return false;
                    }
                    return true;
                });
                action2->AddParameter_("name", Tools::DValue(""), true)
                ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                {
                    if(param->get_value().get_type() != Tools::DValue::Type::kString)
                    {
                        param->set_error("El nombre debe ser una cadena de texto");
                        return false;
                    }
                    if(param->get_value().ToString_() == "")
                    {
                        param->set_error("El identifnombreicador no puede estar vacío");
                        return false;
                    }
                    if(param->get_value().ToString_().size() < 3)
                    {
                        param->set_error("El nombre no puede ser menor a 3 dígitos");
                        return false;
                    }
                    return true;
                });
                action2->AddParameter_("state", Tools::DValue(""), true)
                ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                {
                    if(param->get_value().get_type() != Tools::DValue::Type::kString)
                    {
                        param->set_error("El estado debe ser una cadena de texto");
                        return false;
                    }
                    if(param->get_value().ToString_() == "")
                    {
                        param->set_error("El estado no puede estar vacío");
                        return false;
                    }
                    if(param->get_value().ToString_().size() < 3)
                    {
                        param->set_error("El estado no puede ser menor a 3 dígitos");
                        return false;
                    }
                    return true;
                });
                action2->AddParameter_("description", Tools::DValue(""), true);
        }

        void Process_() override
        {
            AddFunctions_();

            // Set security type
            set_security_type(Extras::SecurityType::kDisableAll);
            
            // Process the request body
            ManageRequestBody_();

            // Route identification
            if(!IdentifyRoute_())
            {
                JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint ("+get_method()+") is not available.");
                return;
            }

            /*if(!VerifySession_())
            {
                JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Session not found.");
                return;
            }

            if(!VerifyPermissions_())
            {
                JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
                return;
            }*/

            // Process actions
            ProcessActions_();
        }
};

int main(int argc, char** argv)
{
    // Setup
        Core::NebulaAtom app(true);

        Query::DatabaseManager::StartMySQL_();
        Tools::SettingsManager::ReadBasicProperties_();
        //Security::PermissionsManager::LoadPermissions_();

    // Read sessions
        //Tools::SessionsManager::ReadSessions_();

    // Custom Handler Creator
        app.CustomHandlerCreator_([&](Core::HTTPRequestInfo& info)
        {
            Handlers::RootHandler* handler;

            // Manage the route type
            Tools::Route route(info.uri);

            switch(route.get_current_route_type())
            {
                // Manage Frontend
                case Atom::Tools::RouteType::kEntrypoint:
                {
                    handler = new Webserver;
                    break;
                }

                // Manage Backend
                case Atom::Tools::RouteType::kEndpoint:
                {
                    // Routes
                    Atom::Tools::Route requested_route(info.uri);
                    Atom::Tools::Route login_route({"api", "system", "login"});
                    Atom::Tools::Route logout_route({"api", "system", "logout"});

                    if(requested_route == login_route || requested_route == logout_route)
                        handler = new Handlers::LoginHandler();
                    else
                        handler = new MainBackendHandler;

                    break;
                }

                return handler;
            }

            return handler;
        });

    // Run
        auto code = app.Init_(argc, argv);

    // End
        Query::DatabaseManager::StopMySQL_();
        return code;
}
