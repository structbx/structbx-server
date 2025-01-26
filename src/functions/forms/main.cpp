
#include "functions/forms/main.h"

using namespace StructBX::Functions::Forms;

void MainData::ReadA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action->set_sql_code(
        "SELECT " \
            "f.* " \
        "FROM forms f " \
        "WHERE " \
            "id_space = ? "
    );
    action->AddParameter_("id_space", get_space_id(), false);
}

void MainData::ReadSpecificA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms WHERE id = ? AND id_space = ?");

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void MainData::ReadSpecificA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT * FROM forms WHERE identifier = ? AND id_space = ?");

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);

}

void MainData::AddA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador para este espacio ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id_space", get_space_id(), false);
}

void MainData::AddA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("INSERT INTO forms (identifier, name, state, privacity, description, id_space) VALUES (?, ?, ?, ?, ?, ?)");

    action_->AddParameter_("identifier", "", true)
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
    action_->AddParameter_("name", "", true)
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
    action_->AddParameter_("state", "", true);
    action_->AddParameter_("privacity", "", true);
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}

void MainData::AddA03::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO forms_columns (identifier, name, length, required, id_column_type, id_form) " \
        "SELECT " \
            "? " \
            ",? " \
            ",? " \
            ",? " \
            ",(SELECT id FROM forms_columns_types WHERE identifier = 'int-number') " \
            ",(SELECT id FROM forms WHERE identifier = ? and id_space = ?) "
    );

    action_->AddParameter_("identifier", "id", false);
    action_->AddParameter_("name", "ID", false);
    action_->AddParameter_("length", "11", false);
    action_->AddParameter_("required", 1, false);
    action_->AddParameter_("identifier", "", true);
    action_->AddParameter_("space_id", get_space_id(), false);
}

void MainData::AddA03_1::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO forms_permissions (`read`, `add`, `modify`, `delete`, id_form, id_naf_user) " \
        "SELECT 1, 1, 1, 1 " \
            ",(SELECT id FROM forms WHERE identifier = ? and id_space = ?) " \
            ",? "
    );

    action_->AddParameter_("identifier", "", true);
    action_->AddParameter_("space_id", get_space_id(), false);
    action_->AddParameter_("user_id", get_id_user(), false);
}

void MainData::ModifyA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier, id_space FROM forms WHERE id = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void MainData::ModifyA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id != ? AND id_space = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador en este espacio ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("space_id", get_space_id(), false);
}

void MainData::ModifyA03::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE forms " \
        "SET identifier = ?, name = ?, state = ?, privacity = ?, description = ? " \
        "WHERE id = ? AND id_space = ?"
    );

    // Parameters and conditions
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
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
        bool result = Tools::IDChecker().Check_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9 y \"_\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action_->AddParameter_("name", "", true)
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
    action_->AddParameter_("state", "", true);
    action_->AddParameter_("privacity", "", true);
    action_->AddParameter_("description", "", true);

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);

}

void MainData::DeleteA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT identifier FROM forms WHERE id = ?");
    action_->set_final(false);
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void MainData::DeleteA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM forms WHERE id = ? AND id_space = ?");
    action_->AddParameter_("id", "", true);
    action_->AddParameter_("id_space", get_space_id(), false);
}

Main::Main(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
    ,data_(function_data)
    ,columns_(function_data)
    ,permissions_(function_data)
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void Main::Read_()
{
    // Function GET /api/forms/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action1 = function->AddAction_("a1");
    actions_.read_a01_.Setup_(action1);

    // Setup custom process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }

        // Iterate over results
        for(auto row : *action1->get_results())
        {
            // Get form id
            auto id = row->ExtractField_("id");
            if(id->IsNull_())
                continue;

            // Action 2: COUNT
            auto action2 = NAF::Functions::Action("a2");
            action2.set_sql_code(
                "SELECT COUNT(1) AS total " \
                "FROM _structbx_space_" + space_id + "._structbx_form_" + id.get()->ToString_());
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
        auto json_results = action1->CreateJSONResult_();

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_results);
    });

    get_functions()->push_back(function);
}

void Main::ReadSpecific_()
{
    // Function GET /api/forms/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    actions_.read_specific_a01_.Setup_(action);

    get_functions()->push_back(function);

    // Function GET /api/forms/read/identifier
    NAF::Functions::Function::Ptr function2 = 
        std::make_shared<NAF::Functions::Function>("/api/forms/read/identifier", HTTP::EnumMethods::kHTTP_GET);

    auto action2 = function2->AddAction_("a2");
    actions_.read_specific_a02_.Setup_(action2);

    get_functions()->push_back(function2);
}

void Main::Add_()
{
    // Function POST /api/forms/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify that the form identifier don't exists in current space
    auto action1 = function->AddAction_("a1");
    actions_.add_a01_.Setup_(action1);

    // Action 2: Add the new form
    auto action2 = function->AddAction_("a2");
    actions_.add_a02_.Setup_(action2);
    
    // Action 3: Add the ID Column to the form
    auto action3 = function->AddAction_("a3");
    actions_.add_a03_.Setup_(action3);

    // Action 3_1: Add form permissions to current user
    auto action3_1 = function->AddAction_("a3_1");
    actions_.add_a03_1_.Setup_(action3_1);

    // Action 4: Create the table
    auto action4 = function->AddAction_("a4");
    
    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1, action2, action3, action3_1, action4](NAF::Functions::Function& self)
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
        if(!action3_1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action3_1->get_identifier() + ": " + action3_1->get_custom_error());
            return;
        }

        // Form ID
        int form_id = action2->get_last_insert_id();
        if(form_id == 0)
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error MVm2IlbSnm");
            return;
        }

        // Columns ID
        int column_id = action3->get_last_insert_id();
        if(column_id == 0)
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error lyEC9cs1tj");
            return;
        }

        // Action 4: Create the table
        action4->set_sql_code(
            "CREATE TABLE _structbx_space_" + space_id + "._structbx_form_" + std::to_string(form_id) + " " \
            "(" \
                "_structbx_column_" + std::to_string(column_id) + " INT NOT NULL AUTO_INCREMENT PRIMARY KEY " \
            ")"
        );
        if(!action4->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error " + action4->get_identifier() + ": No se pudo crear la tabla");

            // Delete form from table
            NAF::Functions::Action action5("a5");
            action5.set_sql_code("DELETE FROM forms WHERE id = ?");
            action5.AddParameter_("id", std::to_string(form_id), false);

            return;
        }

        // Create the directory to store files
        try
        {
            auto directory = NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded");
            directory += "/" + space_id + "/" + std::to_string(form_id);
            Poco::File file(directory);
            if(file.exists())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
                return;
            }
            if(!file.createDirectory())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos del formulario");
                return;
            }

            // Send results
            self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
        }
        catch(Poco::FileException& e)
        {
            NAF::Tools::OutputLogger::Debug_(e.displayText());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos del formulario");
            return;
        }
        catch(std::exception& e)
        {
            NAF::Tools::OutputLogger::Debug_(e.what());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos del formulario");
            return;
        }

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void Main::Modify_()
{
    // Function PUT /api/forms/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify forms existence
    auto action1 = function->AddAction_("a1");
    actions_.modify_a01_.Setup_(action1);

    // Action 2: Verify that the form identifier don't exists
    auto action2 = function->AddAction_("a2");
    actions_.modify_a02_.Setup_(action2);

    // Action 3: Modify form
    auto action3 = function->AddAction_("a3");
    actions_.modify_a03_.Setup_(action3);

    get_functions()->push_back(function);
}

void Main::Delete_()
{
    // Function DEL /api/forms/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify forms existence
    auto action1 = function->AddAction_("a1");
    actions_.delete_a01_.Setup_(action1);

    // Action 2: Delete form from table
    auto action2 = function->AddAction_("a2");
    actions_.delete_a02_.Setup_(action2);

    // Setup Custom Process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action1, action2](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }
            
        // Variables
        auto id = action1->GetParameter("id");
        if(id == action1->get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error J5pktjAN5K");
            return;
        }

        // Action 3: Drop table
        auto action3 = self.AddAction_("a3");
        action3->set_sql_code("DROP TABLE IF EXISTS _structbx_space_" + space_id + "._structbx_form_" + id->get()->ToString_());
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error lOuU13kOu6, asegúrese que no hayan enlaces creados hacia su formulario");
            return;
        }

        // Action 2: Delete form record
        self.IdentifyParameters_(action2);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error kJ79T9LBRw");
            return;
        }

        // Delete form directory
        try
        {
            auto directory = NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded");
            directory += "/" + space_id + "/" + id->get()->ToString_();
            Poco::File file(directory);
            if(file.exists())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
                return;
            }
            file.remove(true);

            // Send results
            self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
        }
        catch(Poco::FileException& e)
        {
            NAF::Tools::OutputLogger::Debug_(e.displayText());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo borrar el directorio de archivos del formulario");
            return;
        }
        catch(std::exception& e)
        {
            NAF::Tools::OutputLogger::Debug_(e.what());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo borrar el directorio de archivos del formulario");
            return;
        }
        
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}