
#include "functions/organizations.h"

Organizations::Organizations(std::string& username)
{
    Read_();
    ReadSpecific_(username);
    Add_();
    Modify_();
    Delete_();
}

void Organizations::Read_()
{

}

void Organizations::ReadSpecific_(std::string& username)
{
    // Function GET /api/reports/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/general/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT co.id AS id, co.name AS name, co.state AS state, co.logo AS logo, co.description AS description, co.created_at AS created_at " \
        "FROM cloud_organizations co " \
        "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
        "WHERE cu.email = ?"
    );

    // Parameters and conditions
    action->AddParameter_("email", Tools::DValue(username), false);

    functions_.push_back(function);
}

void Organizations::Add_()
{

}

void Organizations::Modify_()
{
    
}

void Organizations::Delete_()
{
    
}