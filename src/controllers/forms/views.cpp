
#include "controllers/forms/views.h"

using namespace StructBX::Controllers::Forms;

Views::Views(Tools::FunctionData& function_data) :
    FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_specific_(function_data)
{
    
}

Views::Read::Read(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/views/read
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/views/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    A1(action);

    get_functions()->push_back(function);
}

void Views::Read::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT " \
            "v.id AS id, v.name AS name, v.conditions AS conditions, v.`order` AS 'order', v.id_form AS id_form " \
        "FROM views v " \
        "JOIN forms f ON f.id = v.id_form " \
        "WHERE " \
            "f.id_space = ? AND f.identifier = ? "
    );

    action->AddParameter_("id_space", get_space_id(), false);

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

Views::ReadSpecific::ReadSpecific(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/views/read/id
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/views/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    A1(action);

    get_functions()->push_back(function);
}

void Views::ReadSpecific::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT " \
            "v.id AS id, v.name AS name, v.conditions AS conditions, v.`order` AS 'order', v.id_form AS id_form " \
        "FROM views v " \
        "JOIN forms f ON f.id = v.id_form " \
        "WHERE " \
            "f.id_space = ? AND f.identifier = ? AND v.id = ?"
    );

    action->AddParameter_("id_space", get_space_id(), false);

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de vista no puede estar vacío");
            return false;
        }
        return true;
    });
}
