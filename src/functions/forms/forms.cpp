
#include "functions/forms/forms.h"

StructBI::Functions::Forms::Forms(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void StructBI::Functions::Forms::Read_()
{
    // Function GET /api/forms/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    actions_.forms_.read_a01_.Setup_(action);

    // Setup custom process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](NAF::Functions::Function& self)
    {
        // Search first action
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 5svQHcqqHH");
            return;
        }

        // Iterate over actions
        for(auto action : self.get_actions())
        {
            // Execute action
            if(!action->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": T9eBepMcTA");
                return;
            }
        }

        // Get action1
        auto action1 = self.GetAction_("a1");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error g56U7d8ljv");
            return;
        }
        
        // Iterate over results
        for(auto row : *action1->get()->get_results())
        {
            // Get form identifier
            auto identifier = row->ExtractField_("identifier");
            if(identifier->IsNull_())
                continue;

            // Action 2: COUNT
            auto action2 = NAF::Functions::Action("a2");
            action2.set_sql_code(
                "SELECT COUNT(1) AS total " \
                "FROM form_" + space_id + "_" + identifier->ToString_());
            if(!action2.Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error JNt2Std2sh");
                return;
            }

            // Get results
            auto total = action2.get_results()->First_();
            if(total->IsNull_())
                continue;

            row->AddField_("total", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(total->Int_())));

        }

        // JSON Results
        auto json_results = action1->get()->CreateJSONResult_();

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_results);
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::Forms::ReadSpecific_()
{
    // Function GET /api/forms/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.forms_.read_specific_a01_.Setup_(action);

    get_functions()->push_back(function);

    // Function GET /api/forms/read/identifier
    NAF::Functions::Function::Ptr function2 = 
        std::make_shared<NAF::Functions::Function>("/api/forms/read/identifier", HTTP::EnumMethods::kHTTP_GET);

    auto action2 = function2->AddAction_("a2");
    actions_.forms_.read_specific_a02_.Setup_(action2);

    get_functions()->push_back(function2);
}

void StructBI::Functions::Forms::Add_()
{
    // Function POST /api/forms/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form identifier don't exists
    auto action1 = function->AddAction_("a1");
    actions_.forms_.add_a01_.Setup_(action1);

    // Action 2: Add the new form
    auto action2 = function->AddAction_("a2");
    actions_.forms_.add_a02_.Setup_(action2);
    
    // Action 3: Add the ID Column to the form
    auto action3 = function->AddAction_("a3");
    actions_.forms_.add_a03_.Setup_(action3);
    
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

void StructBI::Functions::Forms::Modify_()
{
    // Function PUT /api/forms/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify forms existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_.modify_a01_.Setup_(action1);

    // Action 2: Verify that the form identifier don't exists
    auto action2 = function->AddAction_("a2");
    actions_.forms_.modify_a02_.Setup_(action2);

    // Action 3: Modify form
    auto action3 = function->AddAction_("a3");
    actions_.forms_.modify_a03_.Setup_(action3);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](NAF::Functions::Function& self)
    {
        // Search first action
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error UjR4f9nmIO");
            return;
        }

        // Iterate over actions
        for(auto action : self.get_actions())
        {
            // Execute action
            if(!action->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": HbdcAS3FsT");
                return;
            }
        }

        // Compare old identifier and new identifier
        auto action1 = self.GetAction_("a1");
        auto action2 = self.GetAction_("a2");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error EgdW7Wg5WV");
            return;
        }
        auto old_identifier = action1->get()->get_results()->ExtractField_(0, 0);
        auto new_identifier = action2->get()->GetParameter("identifier");

        if(old_identifier->IsNull_() || new_identifier == action2->get()->get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 5sVP0geEXI");
            return;
        }

        // Action 4: Modify the table
        if(old_identifier->ToString_() != new_identifier->get()->ToString_())
        {
            auto action4 = self.AddAction_("a4");
            action4->set_sql_code(
                "RENAME TABLE IF EXISTS " \
                "form_" + id_space + "_" + old_identifier->ToString_() + " " \
                "TO form_" + id_space + "_" + new_identifier->get()->ToString_());
            if(!action4->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error iFZCXs2XEN");
                return;
            }
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::Forms::Delete_()
{
    // Function DEL /api/forms/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify forms existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_.delete_a01_.Setup_(action1);

    // Action 2: Delete form from table
    auto action2 = function->AddAction_("a2");
    actions_.forms_.delete_a02_.Setup_(action2);

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](NAF::Functions::Function& self)
    {
        // Search first action
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 7dCZ4eogZ8");
            return;
        }

        // Iterate over actions
        for(auto action : self.get_actions())
        {
            // Execute action
            if(!action->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": XXFwxZhaNV");
                return;
            }
        }

        // Get form identifier
        auto action1 = self.GetAction_("a1");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 9rR0LnKLCa");
            return;
        }
        auto identifier = action1->get()->get_results()->First_();

        if(identifier->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error J5pktjAN5K");
            return;
        }

        // Action 3: Drop table
        auto action3 = self.AddAction_("a3");
        action3->set_sql_code("DROP TABLE IF EXISTS form_" + space_id + "_" + identifier->ToString_());
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error lOuU13kOu6, asegúrese que no hayan enlaces creados hacia su formulario");
            return;
        }

        // Action 2: Delete form record
        auto action2 = self.GetAction_("a2");
        if(action2 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 3EPXm8sG2Q");
            return;
        }
        self.IdentifyParameters_(*action2);
        if(!action2->get()->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error kJ79T9LBRw");
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}