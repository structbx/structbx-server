
#include "functions/organizations.h"

using namespace StructBI::Functions;

Organizations::Organizations(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
}

void Organizations::Read_()
{
    // Function GET /api/organizations/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/organizations/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT * \
        FROM organizations o \
        JOIN organizations_users ou ON ou.id_organization = o.id \
        WHERE ou.id_naf_user = ?"
    );
    action1->AddParameter_("id_user", get_id_user(), false);
    
    get_functions()->push_back(function);
}
