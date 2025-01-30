
#include "functions/general/permissions.h"

using namespace StructBX::Functions::General;

Permissions::Permissions(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_out_group_(function_data)
    ,struct_add_(function_data)
    ,struct_delete_(function_data)
{
    
}

Permissions::Read::Read(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/general/permissions/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/permissions/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Permissions::Read::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT ng.*, e.title AS title "
        "FROM _naf_permissions ng "
        "JOIN endpoints e ON e.endpoint = ng.endpoint "
        "WHERE id_group = ? "
        "ORDER BY e.title ASC"
    );
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

