
#include "functions/forms/columns.h"

using namespace StructBX::Functions::Forms;

Columns::Columns(Tools::FunctionData& function_data) :
    FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_specific_(function_data)
    ,struct_read_types_(function_data)
    ,struct_add_(function_data)
    ,struct_modify_(function_data)
    ,struct_delete_(function_data)
{
    
}

Columns::Read::Read(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/columns/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    A1(action);

    get_functions()->push_back(function);
}

void Columns::Read::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "WHERE " \
            "id_space = ? AND f.identifier = ? " \
        "ORDER BY fc.position ASC"
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

Columns::ReadSpecific::ReadSpecific(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/columns/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    A1(action);

    get_functions()->push_back(function);
}

void Columns::ReadSpecific::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "WHERE " \
            "fc.id = ? AND id_space = ? AND f.identifier = ? " \
        "ORDER BY fc.position ASC"
    );
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });
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

Columns::ReadTypes::ReadTypes(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/columns/types/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/types/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    A1(action);

    get_functions()->push_back(function);
}

void Columns::ReadTypes::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("SELECT * FROM forms_columns_types ");
}

Columns::Add::Add(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function POST /api/forms/columns/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2: Verify that the columns don't exists in the form
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Save the column
    auto action3 = function->AddAction_("a3");
    A3(action3);

    // Action 4: Add the column in the table
    auto action4 = function->AddAction_("a4");

    // Action 5: Get column id from link to
    auto action5 = function->AddAction_("a5");

    // Action 6: Create foreign key if there is a link to
    auto action6 = function->AddAction_("a6");

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1, action2, action3, action4, action5, action6](NAF::Functions::Function& self)
    {
        // If error, delete the column from the table
        auto delete_column_table = [](int column_id)
        {
            // Delete space from table
            NAF::Functions::Action action("action_delete_column");
            action.set_sql_code("DELETE FROM forms_columns WHERE id = ?");
            action.AddParameter_("id", column_id, false);
            action.Work_();
        };

        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
            return;
        }
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action3->get_identifier() + ": " + action3->get_custom_error());
            return;
        }

        // Get form ID
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error nh39HIiJkd");
            return;
        }

        // Get Column ID
        int column_id = action3->get_last_insert_id();
        if(column_id == 0)
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 6eHh9R4Pv5");
            return;
        }

        // Setup column variables
        auto column_setup = ColumnSetup();
        auto variables = ColumnVariables();
        if(!column_setup.Setup(self, variables))
        {
            delete_column_table(column_id);
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error e4GBhN1lxk");
            return;
        }
        std::string space_db = "_structbx_space_" + space_id;
        std::string form_table = "_structbx_form_" + form_id->ToString_();
        std::string column = "_structbx_column_" + std::to_string(column_id);

        // Action 4: Add the column in the table
        action4->set_sql_code(
            "ALTER TABLE " + space_db + "." + form_table + " " +
            "ADD " + column + " " + variables.column_type + variables.length + " " +
            variables.required + " " + variables.default_value
        );
        if(!action4->Work_())
        {
            delete_column_table(column_id);
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action4->get_identifier() + ": " + action4->get_custom_error());
            return;
        }

        // Create foreign key if there is a link to
        if(variables.link_to != "")
        {
            // Get column id from link to
            action5->set_sql_code(
                "SELECT fc.id " \
                "FROM forms_columns fc " \
                "JOIN forms f ON f.id = fc.id_form " \
                "WHERE f.id = ? AND f.id_space = ?"
            );
            action5->AddParameter_("id_form", variables.link_to, false);
            action5->AddParameter_("id_space", space_id, false);
            if(!action5->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action5->get_identifier() + ": No se pudo crear la llave foránea");
                return;
            }

            auto column_id_link = action5->get_results()->First_();
            if(column_id_link->IsNull_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error: No se pudo crear la llave foránea");
                return;
            }

            // Create the foreign key
            action6->set_sql_code(
                "ALTER TABLE " + space_db + "." + form_table + " " +
                "ADD CONSTRAINT _IDX" + column + " " + 
                "FOREIGN KEY (" + column + ") " +
                "REFERENCES " + space_db + "._structbx_form_" + variables.link_to + 
                    "(_structbx_column_" + column_id_link->ToString_() + ") " + 
                    variables.cascade_key_condition
            );
            if(!action6->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action6->get_identifier() + ": No se pudo crear la llave foránea");
                return;
            }
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void Columns::Add::A1(NAF::Functions::Action::Ptr action)
{
    action->set_final(false);
    action->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

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

    action->AddParameter_("id_space", get_space_id(), false);
}

void Columns::Add::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT fc.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.identifier = ? AND f.identifier = ? AND id_space = ?"
    );

    action->SetupCondition_("verify-column-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("La columna solicitada ya existe en el formulario actual");
            return false;
        }

        return true;
    });

    action->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de columna no puede estar vacío");
            return false;
        }
        return true;
    });

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
    action->AddParameter_("id_space", get_space_id(), false);
}

void Columns::Add::A3(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, position, length, required, default_value, description, id_column_type, link_to, id_form) " \
        "SELECT " \
            "?, ? " \
            ",MAX(fc.position) + 1 " \
            ",?, ?, ?, ?, ?, ? " \
            ",f.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );

    action->AddParameter_("identifier", "", true)
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
    action->AddParameter_("name", "", true)
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

    action->AddParameter_("length", "", true);
    action->AddParameter_("required", "", true)
    ->SetupCondition_("condition-required", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "1" || param->get_value()->ToString_() == "0")
        {
            return true;
        }
        else
        {
            param->set_error("El valor de obligatorio debe ser boleano");
            return false;
        }
        return true;
    });
    action->AddParameter_("default_value", "", true);
    action->AddParameter_("description", "", true);
    action->AddParameter_("id_column_type", "", true)
    ->SetupCondition_("condition-id_column_type", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El tipo de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("link_to", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-link_to", Query::ConditionType::kWarning, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()));
        }
        return true;
    });
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_space", get_space_id(), false);
}

Columns::Modify::Modify(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function POST /api/forms/columns/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2: Verify that the columns don't exists in the form
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Update the column
    auto action3 = function->AddAction_("a3");
    A3(action3);

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1, action2, action3](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
            return;
        }

        // Get form ID
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error KOBE4bH6qL");
            return;
        }

        // Get Column ID
        auto column_id = self.GetParameter_("id");
        if(column_id == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error XVtkYKnme6");
            return;
        }

        // Setup column variables
        auto column_setup = ColumnSetup();
        auto variables = ColumnVariables();
        if(!column_setup.Setup(self, variables))
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error WW17KL82QJ");
            return;
        }
        std::string space_db = "_structbx_space_" + space_id;
        std::string form_table = "_structbx_form_" + form_id->ToString_();
        std::string column = "_structbx_column_" + column_id->get()->ToString_();

        // Action 4: Add the column in the table
        auto action4 = self.AddAction_("a4");
        action4->set_sql_code(
            "ALTER TABLE " + space_db + "." + form_table + " " +
            "CHANGE COLUMN `" + column + "` " + column + 
            " " + variables.column_type + variables.length + " " + variables.required +
            " " + variables.default_value
        );
        if(!action4->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action4->get_identifier() + ": " + action4->get_custom_error());
            return;
        }
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action3->get_identifier() + ": " + action3->get_custom_error());
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void Columns::Modify::A1(NAF::Functions::Action::Ptr action)
{
    action->set_final(false);
    action->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

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

    action->AddParameter_("id_space", get_space_id(), false);
}

void Columns::Modify::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT fc.identifier, fc.id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.id != ? AND fc.identifier = ? AND f.identifier = ?"
    );

    action->SetupCondition_("verify-column-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("La columna solicitada ya existe en el formulario actual");
            return false;
        }

        return true;
    });

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de columna no puede estar vacío");
            return false;
        }
        return true;
    });

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

void Columns::Modify::A3(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE forms_columns SET " \
            "identifier = ?, name = ?, length = ?, required = ? " \
            ",default_value = ?, description = ?, id_column_type = ?, link_to = ?, position = ? " \
        "WHERE id = ? AND id_form = (SELECT id FROM forms WHERE identifier = ? AND id_space = ? LIMIT 1)"
    );

    action->AddParameter_("identifier", "", true)
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
    action->AddParameter_("name", "", true)
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
    action->AddParameter_("length", "", true);
    action->AddParameter_("required", "", true)
    ->SetupCondition_("condition-required", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "1" || param->get_value()->ToString_() == "0")
        {
            return true;
        }
        else
        {
            param->set_error("El valor de obligatorio debe ser boleano");
            return false;
        }
        return true;
    });
    action->AddParameter_("default_value", "", true);
    action->AddParameter_("description", "", true);
    action->AddParameter_("id_column_type", "", true)
    ->SetupCondition_("condition-id_column_type", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El tipo de columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("link_to", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-id_column_type", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()));
        }
        return true;
    });
    action->AddParameter_("position", "", true)
    ->SetupCondition_("condition-position", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La posici&oacute;n no puede estar vacía");
            return false;
        }
        return true;
    });
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de la columna no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_space", get_space_id(), false);
}

Columns::Delete::Delete(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/columns/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify column existence
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2_0: Delete foreign key if exists
    auto action2_0 = function->AddAction_("a2_0");

    // Action 2: Delete column from table
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Delete column record
    auto action3 = function->AddAction_("a3");
    A3(action3);

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1, action2_0, action2, action3](NAF::Functions::Function& self)
    {
        // Execute action 1
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }

        // Get Form ID
        auto form_id = action1->get_results()->front()->ExtractField_("form_id");
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error UOEraAIHjM");
            return;
        }

        // Get Column ID
        auto column_id = action1->get_results()->front()->ExtractField_("column_id");
        if(column_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error UFwgqfZp59");
            return;
        }

        // Action 2_0: Delete foreign key if exists
        action2_0->set_sql_code(
            "ALTER TABLE _structbx_space_" + space_id + "._structbx_form_" + form_id->ToString_() + " " +
            "DROP FOREIGN KEY IF EXISTS _IDX_structbx_column_" + column_id->ToString_());

        // Execute actions
        if(!action2_0->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2_0->get_identifier() + ": " + action2_0->get_custom_error());
            return;
        }

        // Action 2: Delete columns
        action2->set_sql_code(
            "ALTER TABLE _structbx_space_" + space_id + "._structbx_form_" + form_id->ToString_() + " " +
            "DROP COLUMN IF EXISTS _structbx_column_" + column_id->ToString_());

        // Execute actions
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
            return;
        }
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action3->get_identifier() + ": " + action3->get_custom_error());
            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void Columns::Delete::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT fc.id AS column_id, f.id AS form_id " \
        "FROM forms_columns fc " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE fc.id = ? AND f.identifier = ? AND f.id_space = ?"
    );
    action->set_final(false);
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("La columna solicitada no existe en el formulario actual");
            return false;
        }

        return true;
    });

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Columns::Delete::A2(NAF::Functions::Action::Ptr)
{
    
}

void Columns::Delete::A3(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("DELETE FROM forms_columns WHERE id = ?");

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de columna no puede estar vacío");
            return false;
        }
        return true;
    });
}

bool Columns::ColumnSetup::Setup(NAF::Functions::Function& self, ColumnVariables& variables)
{
    // link_to parameter setup
    auto end = self.get_parameters().end();
    auto link_to = self.GetParameter_("link_to");
    if(link_to == end)
    {
        // Add null value link_to
        self.get_parameters().push_back(
            Query::Parameter::Ptr(
                new Query::Parameter("link_to", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), false)
            )
        );
        auto end = self.get_parameters().end();
        link_to = self.GetParameter_("link_to");
        if(link_to == end)
            return false;
    }

    // Get parameters
    auto identifier = self.GetParameter_("identifier");
    auto name = self.GetParameter_("name");
    auto length = self.GetParameter_("length");
    auto required = self.GetParameter_("required");
    auto default_value = self.GetParameter_("default_value");
    auto id_column_type = self.GetParameter_("id_column_type");
    auto form_identifier = self.GetParameter_("form-identifier");
    if(
        identifier == end || name == end || length == end || required == end ||
        default_value == end || id_column_type == end || form_identifier == end
    )
        return false;

    // Column Type setup
    std::string column_type_id = id_column_type->get()->ToString_();
    auto column_type_setup = ColumnTypeSetup();
    if(!column_type_setup.Setup(column_type_id, variables.column_type, variables.length))
        return false;

    // Length setup
    if(length->get()->ToString_() != "")
        variables.length = "(" + length->get()->ToString_() + ")";

    // Required setup
    if(required->get()->ToString_() == "1")
    {
        variables.required = "NOT NULL";
        variables.cascade_key_condition = "ON DELETE CASCADE ON UPDATE CASCADE";
    }

    // Default setup
    if(default_value->get()->ToString_() != "")
        variables.default_value = "DEFAULT " + default_value->get()->ToString_();

    // Link to
    if(!link_to->get()->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kEmpty) && link_to->get()->ToString_() != "")
        variables.link_to = link_to->get()->ToString_();

    return true;
}

bool Columns::ColumnTypeSetup::Setup(std::string column_type_id, std::string& column_type, std::string& length_value)
{
    if(column_type_id == "1" || column_type_id == "10" || column_type_id == "11")
    {
        column_type = "VARCHAR";
        length_value = "(100)";
        return true;
    }
    else if(column_type_id == "2" || column_type_id == "7" || column_type_id == "8")
    {
        column_type = "TEXT";
        length_value = "";
        return true;
    }
    else if(column_type_id == "3")
    {
        column_type = "INT";
        length_value = "(11)";
        return true;
    }
    else if(column_type_id == "4")
    {
        column_type = "DECIMAL";
        length_value = "(10, 2)";
        return true;
    }
    else if(column_type_id == "5")
    {
        column_type = "DATE";
        length_value = "";
        return true;
    }
    else if(column_type_id == "6")
    {
        column_type = "TIME";
        length_value = "";
        return true;
    }
    else if(column_type_id == "9")
    {
        column_type = "INT";
        length_value = "(11)";
        return true;
    }
    else if(column_type_id == "12" || column_type_id == "13")
    {
        column_type = "DATETIME";
        length_value = "";
        return true;
    }
    else
    {
        return false;
    } 
}