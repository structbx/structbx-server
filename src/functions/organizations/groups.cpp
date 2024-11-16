
#include "functions/organizations/groups.h"

using namespace StructBI::Functions::Organizations;

Groups::Groups(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
}

void Groups::Read_()
{
    // Function GET /api/organizations/groups/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/organizations/groups/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT ng.* "
        "FROM _naf_groups ng "
    );

    get_functions()->push_back(function);
}
