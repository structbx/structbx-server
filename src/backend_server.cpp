
#include "backend_server.h"

BackendServer::BackendServer() :
    space_id_cookie_("1f3efd18688d2b844f4fa1e800712c9b5750c031", "")
    ,add_space_id_cookie_(false)
{

}

void BackendServer::AddFunctions_()
{
    // Spaces
    auto spaces = Functions::Spaces(function_data_);
    for(auto it : *spaces.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    
    // Spaces Users
    /*auto spaces_users = SpacesUsers(function_data_);
    for(auto it : *spaces_users.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
    
    // Spaces Logo
    auto spaces_logo = SpacesLogo(function_data_);
    for(auto it : *spaces_logo.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));*/
    
    // Forms
    auto forms = Functions::Forms(function_data_);
    for(auto it : *forms.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));

    // Forms Data
    auto forms_data = Functions::FormsData(function_data_);
    for(auto it : *forms_data.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));

    // Forms Columns
    auto forms_columns = Functions::FormsColumns(function_data_);
    for(auto it : *forms_columns.get_functions())
        get_functions_manager().get_functions().insert(std::make_pair(it->get_endpoint(), it));
}

void BackendServer::Process_()
{
    get_files_parameters()->set_directory_base(NAF::Tools::SettingsManager::GetSetting_("directory_base", "/var/www"));
    
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

    // Setup Function Data
    SetupFunctionData_();

    // Add functions
    AddFunctions_();

    // Route identification
    if(!IdentifyRoute_())
    {
        JSONResponse_(HTTP::Status::kHTTP_NOT_FOUND, "The requested endpoint (" + get_properties().method + ") is not available.");
        return;
    }

    // Setup space id cookie
    if(add_space_id_cookie_)
        get_current_function()->AddCookie_(space_id_cookie_);

    // Verify permissions
    if(!VerifyPermissions_())
    {
        JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "The user does not have the permissions to perform this operation.");
        return;
    }

    // Process actions
    ProcessActions_();
}

void BackendServer::SetupFunctionData_()
{
    // Setup User ID
    function_data_.set_id_user(get_users_manager().get_current_user().get_id());
    
    // Get Cookie Space ID
    Poco::Net::NameValueCollection cookies;
    get_http_server_request().value()->getCookies(cookies);
    auto cookie_space_id = cookies.find("1f3efd18688d2b844f4fa1e800712c9b5750c031");

    // Set Space ID if exists in Cookies
    if(cookie_space_id != cookies.end())
    {
        auto space_id_decoded = Tools::Base64Tool().Decode_(cookie_space_id->second);
        function_data_.set_space_id(space_id_decoded);
    }
    else
    {
        add_space_id_cookie_ = true;

        // Get Space ID Cookie if not exists in Cookies
        auto action = NAF::Functions::Action("a1");
        action.set_sql_code(
            "SELECT s.id " \
            "FROM spaces s " \
            "JOIN spaces_users su ON su.id_space = s.id " \
            "WHERE su.id_naf_user = ? LIMIT 1"
        );
        action.AddParameter_("id_naf_user", function_data_.get_id_user(), false);
        if(action.Work_())
        {
            auto space_id = action.get_results()->First_();
            if(!space_id->IsNull_())
            {
                // Set Space ID
                function_data_.set_space_id(space_id->ToString_());

                // Save Space ID to Cookie
                auto space_id_encoded = Tools::Base64Tool().Encode_(space_id->ToString_());

                Net::HTTPCookie cookie("1f3efd18688d2b844f4fa1e800712c9b5750c031", space_id_encoded);
                cookie.setPath("/");
                cookie.setSameSite(Net::HTTPCookie::SAME_SITE_STRICT);
                cookie.setSecure(true);
                cookie.setHttpOnly();
                space_id_cookie_ = HTTP::Cookie(cookie);
            }
        }
    }
}