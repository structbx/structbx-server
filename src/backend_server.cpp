
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

    // Reports Graphs
    auto reports_graphs = ReportsGraphs();
    for(auto it : reports_graphs.get_functions())
    {
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    }

    // Reports parameters
    auto reports_parameters = ReportsParameters();
    for(auto it : reports_parameters.get_functions())
    {
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    }

    // Organizations
    auto username = get_users_manager().get_current_user().get_username();
    auto organizations = Organizations(username);
    for(auto it : organizations.get_functions())
    {
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    }
}

void BackendServer::Process_()
{
    // Set security type
    set_security_type(Extras::SecurityType::kDisableAll);
    
    // Process the request body
    ManageRequestBody_();

    // Verify sessions
    if(!VerifySession_())
    {
        JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Session not found.");
        return;
    }

    // Add functions
    AddFunctions_();

    // Route identification
    if(!IdentifyRoute_())
    {
        JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint (" + get_properties().method + ") is not available.");
        return;
    }

    // Verify permissions
    if(!VerifyPermissions_())
    {
        JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
        return;
    }

    // Process actions
    ProcessActions_();
}