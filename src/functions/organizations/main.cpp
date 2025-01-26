
#include "functions/organizations/main.h"
#include "functions/organizations/users.h"

using namespace StructBX::Functions::Organizations;

Main::Main(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,users_(function_data)
    ,groups_(function_data)
{
    Read_();
    Modify_();
}

void Main::Read_()
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

void Main::Modify_()
{
    // Function GET /api/organizations/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/organizations/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "UPDATE organizations o \
        JOIN organizations_users ou ON ou.id_organization = o.id \
        SET o.name = ? \
        WHERE ou.id_naf_user = ?"
    );
    action1->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() <= 3)
        {
            param->set_error("El nombre no puede tener menos de 3 caracteres");
            return false;
        }
        return true;
    });
    action1->AddParameter_("id_user", get_id_user(), false);
    
    get_functions()->push_back(function);
}
