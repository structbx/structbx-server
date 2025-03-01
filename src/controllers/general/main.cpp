
#include "controllers/general/main.h"

using namespace StructBX::Controllers::General;

Main::Main(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,function_users_(function_data)
    ,function_groups_(function_data)
    ,function_permissions_(function_data)
    ,struct_read_instance_name_(function_data)
    ,struct_modify_instance_name_(function_data)
{
    
}

Main::ReadInstanceName::ReadInstanceName(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/read
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/general/instanceName/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT * \
        FROM settings \
        WHERE name = 'instance_name'"
    );
    
    get_functions()->push_back(function);
}

Main::ModifyInstanceName::ModifyInstanceName(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/modify
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/general/instanceName/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "UPDATE settings \
        SET value = ? \
        WHERE name = 'instance_name'"
    );
    action1->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de la instancia no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() <= 3)
        {
            param->set_error("El nombre de la instancia no puede tener menos de 3 caracteres");
            return false;
        }
        return true;
    });
    
    get_functions()->push_back(function);
}
