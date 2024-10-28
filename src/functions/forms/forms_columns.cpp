
#include "functions/forms/forms_columns.h"

StructBI::Functions::FormsColumns::FormsColumns(Tools::FunctionData& function_data) :
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

void StructBI::Functions::FormsColumns::Read_()
{
    // Function GET /api/forms/columns/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_columns_.read_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsColumns::ReadSpecific_()
{
    // Function GET /api/forms/columns/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_columns_.read_specific_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsColumns::ReadTypes_()
{
    // Function GET /api/forms/columns/types/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/types/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_columns_.read_types_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsColumns::Add_()
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
            
        // Get parameters
        auto end = self.get_parameters().end();
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
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error xTWsdae5pJ");
            return;
        }

        // Column Type Transformations
        std::string column_type = "VARCHAR";
        std::string length_value = "(100)";
        if(id_column_type->get()->ToString_() == "1")
        {
            column_type = "VARCHAR";
            length_value = "(100)";
        }
        else if(id_column_type->get()->ToString_() == "2")
        {
            column_type = "TEXT";
            length_value = "";
        }
        else if(id_column_type->get()->ToString_() == "3")
        {
            column_type = "INT";
            length_value = "(11)";
        }
        else if(id_column_type->get()->ToString_() == "4")
        {
            column_type = "DECIMAL";
            length_value = "(10, 2)";
        }
        else if(id_column_type->get()->ToString_() == "5")
        {
            column_type = "DATETIME";
            length_value = "";
        }
        else if(id_column_type->get()->ToString_() == "5")
        {
            column_type = "TIME";
            length_value = "";
        }
        else
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error HetZIhSqTe");
            return;
        }

        // Length Transformations
        if(length->get()->ToString_() != "")
            length_value = "(" + length->get()->ToString_() + ")";

        // Required Transformations
        std::string required_value = "";
        if(required->get()->ToString_() == "1")
            required_value = "NOT NULL";

        // Action 4: Add the column in the table
        auto action4 = self.AddAction_("a4");
        action4->set_sql_code(
            "ALTER TABLE _structbi_space_" + space_id + "._structbi_form_" + form_id->ToString_() + " " +
            "ADD _structbi_column_" + std::to_string(column_id) + " " + column_type + length_value + " " +
            required_value + default_value->get()->ToString_()
        );
        if(!action4->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action4->get_identifier() + ": " + action4->get_custom_error());
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsColumns::Modify_()
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
        
        // Get parameters
        auto end = self.get_parameters().end();
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
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error H1AZ0gYu7T");
            return;
        }

        // Column Type Transformations
        std::string column_type = "VARCHAR";
        std::string length_value = "(100)";
        if(id_column_type->get()->ToString_() == "1")
        {
            column_type = "VARCHAR";
            length_value = "(100)";
        }
        else if(id_column_type->get()->ToString_() == "2")
        {
            column_type = "TEXT";
            length_value = "";
        }
        else if(id_column_type->get()->ToString_() == "3")
        {
            column_type = "INT";
            length_value = "(11)";
        }
        else if(id_column_type->get()->ToString_() == "4")
        {
            column_type = "DECIMAL";
            length_value = "(10, 2)";
        }
        else if(id_column_type->get()->ToString_() == "5")
        {
            column_type = "DATE";
            length_value = "";
        }
        else if(id_column_type->get()->ToString_() == "5")
        {
            column_type = "TIME";
            length_value = "";
        }
        else
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error Q5FLHrDF3H");
            return;
        }

        // Length Transformations
        if(length->get()->ToString_() != "")
            length_value = "(" + length->get()->ToString_() + ")";

        // Required Transformations
        std::string required_value = "";
        if(required->get()->ToString_() == "1")
            required_value = "NOT NULL";

        // Action 4: Add the column in the table
        auto action4 = self.AddAction_("a4");
        action4->set_sql_code(
            "ALTER TABLE _structbi_space_" + space_id + "._structbi_form_" + form_id->ToString_() + " " +
            "CHANGE COLUMN `_structbi_column_" + column_id->get()->ToString_() + "` _structbi_column_" + column_id->get()->ToString_() + 
            " " + column_type + length_value + " " + required_value + " " + default_value->get()->ToString_()
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

void StructBI::Functions::FormsColumns::Delete_()
{
    // Function GET /api/forms/columns/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/columns/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify column existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_columns_.delete_a01_.Setup_(action1);

    // Action 2: Delete column from table
    auto action2 = function->AddAction_("a2");
    actions_.forms_columns_.delete_a02_.Setup_(action2);

    // Action 3: Delete column record
    auto action3 = function->AddAction_("a3");
    actions_.forms_columns_.delete_a03_.Setup_(action3);

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1, action2, action3](NAF::Functions::Function& self)
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

        // Action 2: Delete columns
        action2->set_sql_code(
            "ALTER TABLE _structbi_space_" + space_id + "._structbi_form_" + form_id->ToString_() + " " +
            "DROP COLUMN _structbi_column_" + column_id->ToString_());

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
