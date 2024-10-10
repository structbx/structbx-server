
#include "functions/spaces/spaces_users.h"

SpacesUsers::SpacesUsers(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
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

    get_functions()->push_back(function);
}
