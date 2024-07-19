
#include "core/nebula_atom.h"
#include "functions/action.h"
#include "handlers/custom_handler.h"
#include "handlers/frontend_handler.h"
#include "handlers/root_handler.h"
#include "tools/route.h"
#include <query/parameter.h>
#include <tools/dvalue.h>

#include "web_server.h"
#include "backend_server.h"

using namespace Atom;

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
                        handler = new BackendServer;

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
