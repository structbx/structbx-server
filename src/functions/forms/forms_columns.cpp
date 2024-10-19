
#include "functions/forms/forms_columns.h"

StructBI::Functions::FormsColumns::FormsColumns(Tools::FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    ReadTypes_();
    Add_();
}

void StructBI::Functions::FormsColumns::Read_()
{
    // Function GET /api/forms/columns/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read", HTTP::EnumMethods::kHTTP_GET);

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

void StructBI::Functions::FormsColumns::ReadTypes_()
{
    // Function GET /api/forms/columns/types/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/types/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM forms_columns_types ");

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsColumns::Add_()
{
    // Function POST /api/forms/columns/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form identifier don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_final(false);
    action1->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El formulario no existe o es erroneo");
            return false;
        }

        return true;
    });

    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id_space", get_space_id(), false);

    // Action 2: Add the new column
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, length, required, default_value, description, id_column_type, id_form, id_space) " \
        "SELECT ?, ?, ?, ?, ?, ?, (SELECT id FROM forms WHERE identifier = ? AND id_space = ?)"
    );

    action2->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value()->ToString_();
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(string_param == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(string_param.size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action2->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action2->AddParameter_("state", "", true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("privacity", "", true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action2->AddParameter_("description", "", true);
    action2->AddParameter_("id_space", get_space_id(), false);

    // Action 2_1: Add the ID Column to the form
    auto action2_1 = function->AddAction_("a2_1");
    action2_1->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, length, required, id_column_type, id_form) " \
        "SELECT " \
            "? " \
            ",? " \
            ",? " \
            ",? " \
            ",(SELECT id FROM forms_columns_types WHERE identifier = 'int-number') " \
            ",(SELECT id FROM forms WHERE identifier = ? and id_space = ?) "
    );

    action2_1->AddParameter_("identifier", "id", false);
    action2_1->AddParameter_("name", "ID", false);
    action2_1->AddParameter_("length", "11", false);
    action2_1->AddParameter_("required", 1, false);
    action2_1->AddParameter_("identifier", "", true);
    action2_1->AddParameter_("space_id", get_space_id(), false);

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](NAF::Functions::Function& self)
    {
        // Search first action
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "No actions found.");
            return;
        }

        // Iterate over actions
        for(auto action : self.get_actions())
        {
            // Execute action
            if(!action->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action->get_custom_error());
                return;
            }
        }

        // Get form identifier
        auto form_identifier = self.GetParameter_("identifier");

        // Action 3: Create the table
        auto action3 = self.AddAction_("a3");
        action3->set_sql_code(
            "CREATE TABLE form_" + space_id + "_" + form_identifier->get()->get_value()->ToString_() + " " \
            "(" \
                "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY " \
            ")"
        );
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action3->get_custom_error());
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}
