
#include "functions/spaces/users.h"

using namespace StructBI::Functions::Spaces;

Users::Users(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    /*Add_();
    Delete_();*/
}

void Users::Read_()
{
    // Function GET /api/spaces/users/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/users/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.username, sp.created_at " \
        "FROM _naf_users nu " \
        "JOIN spaces_users sp ON sp.id_naf_user = nu.id " \
        "WHERE sp.id_space = ?"
    );
    action1->AddParameter_("id_space", get_space_id(), true)
    ->SetupCondition_("condition-id_space", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de espacio no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}
