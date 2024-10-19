
#include "functions/forms/forms_columns.h"
#include "tools/actions_data.h"

StructBI::Functions::FormsColumns::FormsColumns(Tools::FunctionData& function_data) :
    FunctionData(function_data)
    ,actions_(function_data)
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
    actions_.forms_columns_.read_a01_.Setup_(action);

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

    // Action 1: Verify that the columns don't exists in the form
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
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action1->get_custom_error());
            return;
        }
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action2->get_custom_error());
            return;
        }
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action3->get_custom_error());
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
            std::string length_value = "(100)";
        }
        else if(id_column_type->get()->ToString_() == "2")
        {
            column_type = "TEXT";
            std::string length_value = "";
        }
        else if(id_column_type->get()->ToString_() == "3")
        {
            column_type = "INT";
            std::string length_value = "(11)";
        }
        else if(id_column_type->get()->ToString_() == "4")
        {
            column_type = "DECIMAL";
            std::string length_value = "(10, 2)";
        }
        else if(id_column_type->get()->ToString_() == "5")
        {
            column_type = "DATETIME";
            std::string length_value = "";
        }
        else if(id_column_type->get()->ToString_() == "5")
        {
            column_type = "TIME";
            std::string length_value = "";
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
            "ALTER TABLE form_" + space_id + "_" + form_identifier->get()->get_value()->ToString_() + " " +
            "ADD " + identifier->get()->ToString_() + " " + column_type + length_value + " " +
            required_value + default_value->get()->ToString_()
        );
        if(!action4->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action4->get_custom_error());
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}
