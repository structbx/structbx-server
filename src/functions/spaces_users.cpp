
#include "functions/spaces_users.h"

SpacesUsers::SpacesUsers(int id_user) :
    id_user_(id_user)
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void SpacesUsers::Read_()
{
    // Function GET /api/spaces/users/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/users/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT nu.username, nu.status, nu.created_at, ng.id AS id_group, ng.`group` AS 'group' " \
        "FROM _naf_users nu " \
        "JOIN _naf_groups ng ON ng.id = nu.id_group"
    );

    functions_.push_back(function);
}

void SpacesUsers::ReadSpecific_()
{

}

void SpacesUsers::Add_()
{

}

void SpacesUsers::Modify_()
{
    
}

void SpacesUsers::Delete_()
{
    
}