
#include "functions/forms.h"
#include <algorithm>

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

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT " \
            "* " \
        "FROM forms " \
        "WHERE " \
            "id_space = ? "
    );
    action->AddParameter_("id_space", Tools::DValue(get_space_id()), false);

    get_functions()->push_back(function);
}

void Forms::ReadSpecific_()
{
    // Function GET /api/forms/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM forms WHERE id = ?");

    // Parameters and conditions
    action->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
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
    action1->set_sql_code("SELECT id FROM forms WHERE identifier = ?");
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Add the new form
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("INSERT INTO forms (identifier, name, state, privacity, description, id_space) VALUES (?, ?, ?, ?, ?, ?)");

    action2->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value().ToString_();
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
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
        bool result = IDChecker().Check_(param->get_value().ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action2->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action2->AddParameter_("state", Tools::DValue(""), true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("privacity", Tools::DValue(""), true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action2->AddParameter_("description", Tools::DValue(""), true);
    action2->AddParameter_("id_space", Tools::DValue(get_space_id()), false);

    // Action 3: Get the table id and name
    auto action3 = function->AddAction_("a3");
    action3->set_sql_code("SELECT identifier, id_space FROM forms WHERE id_space = " + get_space_id() + " AND identifier = ?");

    action3->AddParameter_("identifier", Tools::DValue(""), true);

    // Setup Custom Process
    function->SetupCustomProcess_([&](Functions::Function& self)
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

        // Get results of action3
        auto action3 = std::find_if(self.get_actions().begin(), self.get_actions().end(), [](Functions::Action::Ptr& action)
        {
            return action->get_identifier() == "a3";
        });
        if(action3 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Action 3 not found");
            return;
        }
        auto form_identifier = action3->get()->get_results()->ExtractField_(0, 0);
        if(form_identifier->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Action 3 results identifier is null");
            return;
        }
        auto form_id_space = action3->get()->get_results()->ExtractField_(0, 1);
        if(form_id_space->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Action 3 results id space is null");
            return;
        }

        // Action 4: Create the table
        Functions::Action action4("a4");
        action4.set_sql_code(
            "CREATE TABLE form_" + form_id_space->ToString_() + "_" + form_identifier->ToString_() + " " \
            "(" \
                "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, created_at DATETIME NOT NULL DEFAULT NOW() " \
                ",INDEX idx_created_at (created_at) USING BTREE" \
            ")"
        );
        if(!action4.Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action4.get_custom_error());
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

    // Action 1: Get current identifier and id_space
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action1->set_final(false);
    action1->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
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
            self.set_custom_error("Un formulario con este identificador ya existe");
            return false;
        }

        return true;
    });

    action2->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action2->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
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
    action3->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = IDChecker().Check_(param->get_value().ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action3->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action3->AddParameter_("state", Tools::DValue(""), true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action3->AddParameter_("privacity", Tools::DValue(""), true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vacía");
            return false;
        }
        return true;
    });
    action3->AddParameter_("description", Tools::DValue(""), true);

    action3->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
    action3->AddParameter_("id_space", Tools::DValue(get_space_id()), false);

    // Setup Custom Process
    function->SetupCustomProcess_([&](Functions::Function& self)
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
        auto id_space = action1->get()->get_results()->ExtractField_(0, 1);
        auto new_identifier = action2->get()->GetParameter("identifier");

        if(old_identifier->IsNull_() || id_space->IsNull_() || new_identifier == action2->get()->get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 5sVP0geEXI");
            return;
        }

        // Action 4: Modify the table
        if(old_identifier->ToString_() != new_identifier->get()->ToString_())
        {
            Functions::Action action4("a4");
            action4.set_sql_code(
                "RENAME TABLE IF EXISTS " \
                "form_" + id_space->ToString_() + "_" + old_identifier->ToString_() + " " \
                "TO form_" + id_space->ToString_() + "_" + new_identifier->get()->ToString_());
            if(!action4.Work_())
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

    // Action 1: Get current identifier and id_space
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action1->set_final(false);
    action1->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Delete form record
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("DELETE FROM forms WHERE id = ? AND id_space = ?");
    action2->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("id_space", Tools::DValue(get_space_id()), false);

    // Setup Custom Process
    function->SetupCustomProcess_([&](Functions::Function& self)
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
        auto id_space = action1->get()->get_results()->ExtractField_(0, 1);

        if(identifier->IsNull_() || id_space->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error J5pktjAN5K");
            return;
        }

        // Action 3: Delete table
        Functions::Action action3("a3");
        action3.set_sql_code("DROP TABLE IF EXISTS form_" + id_space->ToString_() + "_" + identifier->ToString_());
        if(!action3.Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error lOuU13kOu6");
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}