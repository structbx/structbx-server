
#include "functions/general/groups.h"
#include <http/methods.h>

using namespace StructBX::Functions::General;

Groups::Groups(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_specific_(function_data)
    ,struct_add_(function_data)
    ,struct_modify_(function_data)
    ,struct_delete_(function_data)
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

Groups::ReadSpecific::ReadSpecific(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/general/groups/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/groups/read/id", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Groups::ReadSpecific::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT ng.* "
        "FROM _naf_groups ng "
        "WHERE ng.id = ?"
    );

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

Groups::Add::Add(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function POST /api/general/groups/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/groups/add", HTTP::EnumMethods::kHTTP_POST);
    
    // Verify if group new name don't exists yet
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

Groups::Modify::Modify(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function PUT /api/general/groups/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/groups/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    // Verify if group don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Verify if group new name don't exists yet
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Modify group
    auto action3 = function->AddAction_("a3");
    A3(action3);

    get_functions()->push_back(function);
}

void Groups::Modify::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT id "
        "FROM _naf_groups "
        "WHERE id = ?"
    );
    action->SetupCondition_("condition-group-exists", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El grupo al que intenta modificar no existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

void Groups::Modify::A2(NAF::Functions::Action::Ptr action)
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
            self.set_custom_error("Este nombre de grupo ya está registrado");
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

void Groups::Modify::A3(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("UPDATE _naf_groups SET `group` = ? WHERE id = ?");
    action->AddParameter_("group", "", true);
    action->AddParameter_("id", "", true);

}

Groups::Delete::Delete(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function PUT /api/general/groups/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/groups/delete", HTTP::EnumMethods::kHTTP_DEL);
    
    // Verify if group don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Delete group
    auto action2 = function->AddAction_("a2");
    A2(action2);

    get_functions()->push_back(function);
}

void Groups::Delete::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT id "
        "FROM _naf_groups "
        "WHERE id = ?"
    );
    action->SetupCondition_("condition-group-exists", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El grupo al que intenta borrar no existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

void Groups::Delete::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("DELETE FROM _naf_groups WHERE id = ?");
    action->AddParameter_("id", "", true);

}