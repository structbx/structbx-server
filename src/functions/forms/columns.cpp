
#include "functions/forms/columns.h"

using namespace StructBI::Functions::Forms;

Columns::Columns(Tools::FunctionData& function_data) :
    FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    ReadSpecific_();
    ReadTypes_();
    Add_();
    Modify_();
    Delete_();
}

void Columns::Read_()
{
    // Function GET /api/forms/columns/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_columns_.read_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void Columns::ReadSpecific_()
{
    // Function GET /api/forms/columns/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_columns_.read_specific_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void Columns::ReadTypes_()
{
    // Function GET /api/forms/columns/types/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/types/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_columns_.read_types_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void Columns::Add_()
{
    // Function POST /api/forms/columns/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form exists
    auto action1 = function->AddAction_("a1");
    actions_.forms_columns_.add_a01_.Setup_(action1);

    // Action 2: Verify that the columns don't exists in the form
    auto action2 = function->AddAction_("a2");
    actions_.forms_columns_.add_a02_.Setup_(action2);

    // Action 3: Save the column
    auto action3 = function->AddAction_("a3");
    actions_.forms_columns_.add_a03_.Setup_(action3);

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
        std::string space_db = "_structbi_space_" + space_id;
        std::string form_table = "_structbi_form_" + form_id->ToString_();
        std::string column = "_structbi_column_" + std::to_string(column_id);

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
                "REFERENCES " + space_db + "._structbi_form_" + variables.link_to + 
                    "(_structbi_column_" + column_id_link->ToString_() + ") " + 
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

void Columns::Modify_()
{
    // Function POST /api/forms/columns/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form exists
    auto action1 = function->AddAction_("a1");
    actions_.forms_columns_.modify_a01_.Setup_(action1);

    // Action 2: Verify that the columns don't exists in the form
    auto action2 = function->AddAction_("a2");
    actions_.forms_columns_.modify_a02_.Setup_(action2);

    // Action 3: Update the column
    auto action3 = function->AddAction_("a3");
    actions_.forms_columns_.modify_a03_.Setup_(action3);

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
        std::string space_db = "_structbi_space_" + space_id;
        std::string form_table = "_structbi_form_" + form_id->ToString_();
        std::string column = "_structbi_column_" + column_id->get()->ToString_();

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

void Columns::Delete_()
{
    // Function GET /api/forms/columns/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify column existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_columns_.delete_a01_.Setup_(action1);

    // Action 2_0: Delete foreign key if exists
    auto action2_0 = function->AddAction_("a2_0");

    // Action 2: Delete column from table
    auto action2 = function->AddAction_("a2");
    actions_.forms_columns_.delete_a02_.Setup_(action2);

    // Action 3: Delete column record
    auto action3 = function->AddAction_("a3");
    actions_.forms_columns_.delete_a03_.Setup_(action3);

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
            "ALTER TABLE _structbi_space_" + space_id + "._structbi_form_" + form_id->ToString_() + " " +
            "DROP FOREIGN KEY IF EXISTS _IDX_structbi_column_" + column_id->ToString_());

        // Execute actions
        if(!action2_0->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2_0->get_identifier() + ": " + action2_0->get_custom_error());
            return;
        }

        // Action 2: Delete columns
        action2->set_sql_code(
            "ALTER TABLE _structbi_space_" + space_id + "._structbi_form_" + form_id->ToString_() + " " +
            "DROP COLUMN IF EXISTS _structbi_column_" + column_id->ToString_());

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

bool Columns::ColumnSetup::Setup(NAF::Functions::Function& self, ColumnVariables& variables)
{
    // Get parameters
    auto end = self.get_parameters().end();
    auto identifier = self.GetParameter_("identifier");
    auto name = self.GetParameter_("name");
    auto length = self.GetParameter_("length");
    auto required = self.GetParameter_("required");
    auto default_value = self.GetParameter_("default_value");
    auto id_column_type = self.GetParameter_("id_column_type");
    auto link_to = self.GetParameter_("link_to");
    auto form_identifier = self.GetParameter_("form-identifier");
    if(
        identifier == end || name == end || length == end || required == end ||
        default_value == end || id_column_type == end || link_to == end || form_identifier == end
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
    if(link_to->get()->ToString_() != "")
        variables.link_to = link_to->get()->ToString_();

    return true;
}

bool Columns::ColumnTypeSetup::Setup(std::string column_type_id, std::string& column_type, std::string& length_value)
{
    if(column_type_id == "1")
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
    else
    {
        return false;
    } 
}