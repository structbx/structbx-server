
#include "backend_server.h"

BackendServer::BackendServer()
{

}

void BackendServer::AddFunctions_()
{
    function_data_.set_id_user(get_users_manager().get_current_user().get_id());
    
    // Forms
    auto forms = Forms(function_data_);
    for(auto it : *forms.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));

    // Dashboards
    auto dashboards = Dashboards(function_data_);
    for(auto it : *dashboards.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));

    // Reports
    auto reports = Reports(function_data_);
    for(auto it : *reports.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));

    // Spaces
    auto spaces = Spaces(function_data_);
    for(auto it : *spaces.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
}

void BackendServer::Process_()
{
    get_files_parameters()->set_directory_base(Tools::SettingsManager::GetSetting_("directory_base", "/var/www"));
    
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