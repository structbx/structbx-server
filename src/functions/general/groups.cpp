
#include "functions/general/groups.h"

using namespace StructBX::Functions::General;

Groups::Groups(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_add_(function_data)
{
    
}

Groups::Read::Read(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/general/groups/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/groups/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Groups::Read::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT ng.* "
        "FROM _naf_groups ng "
    );
}

Groups::Add::Add(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/general/groups/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/groups/add", HTTP::EnumMethods::kHTTP_POST);
    
    // Verify if group don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Add group
    auto action2 = function->AddAction_("a2");
    A2(action2);

    get_functions()->push_back(function);
}

void Groups::Add::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT id "
        "FROM _naf_groups "
        "WHERE `group` = ?"
    );
    action->SetupCondition_("condition-group-exists", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este grupo ya está registrado");
            return false;
        }

        return true;
    });

    action->AddParameter_("group", "", true)
    ->SetupCondition_("condition-group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}
void Groups::Add::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("INSERT INTO _naf_groups (`group`) VALUES (?)");
    action->AddParameter_("group", "", true);

}