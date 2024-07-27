
#include "backend_server.h"

void BackendServer::AddFunctions_()
{
    // Forms
    auto forms = Forms();
    for(auto it : forms.get_functions())
    {
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    }

    // Dashboards
    auto dashboards = Dashboards();
    for(auto it : dashboards.get_functions())
    {
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    }

    // Reports
    auto reports = Reports();
    for(auto it : reports.get_functions())
    {
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    }
}

void BackendServer::Process_()
{
    AddFunctions_();

    // Set security type
    set_security_type(Extras::SecurityType::kDisableAll);
    
    // Process the request body
    ManageRequestBody_();

    // Route identification
    if(!IdentifyRoute_())
    {
        JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint (" + get_properties().method + ") is not available.");
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