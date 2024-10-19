
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

    // Action 1: Verify that the form identifier don't exists
    auto action1 = function->AddAction_("a1");
    actions_.forms_columns_.add_a01_.Setup_(action1);

    // Action 2: Add the new column
    auto action2 = function->AddAction_("a2");
    actions_.forms_columns_.add_a02_.Setup_(action2);

    // Action 3: Add the ID Column to the form
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
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action1->get_custom_error());
            return;
        }
            
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action1->get_custom_error());
            return;
        }
            
        // Get form identifier
        auto form_identifier = self.GetParameter_("form-identifier");
        if(form_identifier == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error xTWsdae5pJ");
            return;
        }


        // Action 4: Create the table
        auto action4 = self.AddAction_("a4");
        action4->set_sql_code(
            "CREATE TABLE form_" + space_id + "_" + form_identifier->get()->get_value()->ToString_() + " " \
            "(" \
                "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY " \
            ")"
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
