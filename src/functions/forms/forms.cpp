
#include "functions/forms/forms.h"

Forms::Forms(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void Forms::Read_()
{
    // Function GET /api/forms/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT " \
            "f.* " \
        "FROM forms f " \
        "WHERE " \
            "id_space = ? "
    );
    action->AddParameter_("id_space", get_space_id(), false);

    // Setup custom process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](Functions::Function& self)
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
            auto action2 = Functions::Action("a2");
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

            row->AddField_("total", Tools::DValue::Ptr(new Tools::DValue(total->Int_())));

        }

        // JSON Results
        auto json_results = action1->get()->CreateJSONResult_();

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_results);
    });

    get_functions()->push_back(function);
}

void Forms::ReadSpecific_()
{
    // Function GET /api/forms/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM forms WHERE id = ? AND id_space = ?");

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_space", get_space_id(), false);

    get_functions()->push_back(function);

    // Function GET /api/forms/read/identifier
    Functions::Function::Ptr function2 = 
        std::make_shared<Functions::Function>("/api/forms/read/identifier", HTTP::EnumMethods::kHTTP_GET);

    auto action2 = function2->AddAction_("a1");
    action2->set_sql_code("SELECT * FROM forms WHERE identifier = ? AND id_space = ?");

    action2->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("id_space", get_space_id(), false);

    get_functions()->push_back(function2);
}

void Forms::Add_()
{
    // Function POST /api/forms/add
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form identifier don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_final(false);
    action1->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador para este espacio ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id_space", get_space_id(), false);

    // Action 2: Add the new form
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("INSERT INTO forms (identifier, name, state, privacity, description, id_space) VALUES (?, ?, ?, ?, ?, ?)");

    action2->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value()->ToString_();
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
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
        bool result = IDChecker().Check_(param->get_value()->ToString_());
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
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
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

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](Functions::Function& self)
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

void Forms::Modify_()
{
    // Function PUT /api/forms/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Verify forms existence
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action1->set_final(false);
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Verify that the form identifier don't exists
    auto action2 = function->AddAction_("a2");
    action2->set_final(false);
    action2->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id != ?");
    action2->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador en este espacio ya existe");
            return false;
        }

        return true;
    });

    action2->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action2->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 3: Modify form
    auto action3 = function->AddAction_("a3");
    action3->set_sql_code(
        "UPDATE forms " \
        "SET identifier = ?, name = ?, state = ?, privacity = ?, description = ? " \
        "WHERE id = ? AND id_space = ?"
    );

    // Parameters and conditions
    action3->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->get_value()->ToString_().size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action3->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
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
    action3->AddParameter_("state", "", true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action3->AddParameter_("privacity", "", true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action3->AddParameter_("description", "", true);

    action3->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
    action3->AddParameter_("id_space", get_space_id(), false);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
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

void Forms::Delete_()
{
    // Function DEL /api/forms/delete
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Verify forms existence
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action1->set_final(false);
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Delete form record
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("DELETE FROM forms WHERE id = ? AND id_space = ?");
    action2->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("id_space", get_space_id(), false);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
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

        // Compare old identifier and new identifier
        auto action1 = self.GetAction_("a1");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 9rR0LnKLCa");
            return;
        }
        auto identifier = action1->get()->get_results()->ExtractField_(0, 0);

        if(identifier->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error J5pktjAN5K");
            return;
        }

        // Action 3: Delete table
        auto action3 = self.AddAction_("a3");
        action3->set_sql_code("DROP TABLE IF EXISTS form_" + id_space + "_" + identifier->ToString_());
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error lOuU13kOu6");
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}