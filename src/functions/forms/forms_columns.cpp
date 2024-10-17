
#include "functions/forms/forms_columns.h"

FormsColumns::FormsColumns(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
}

void FormsColumns::Read_()
{
    // Function GET /api/forms/columns/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/columns/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type, fct.name AS column_type_name " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "WHERE " \
            "id_space = ? AND f.identifier = ?"
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

    get_functions()->push_back(function);
}

void FormsColumns::ReadTypes_()
{
    // Function GET /api/forms/columns/types/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/columns/types/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM forms_columns_types ");

    get_functions()->push_back(function);
}

