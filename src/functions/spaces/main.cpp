
#include "functions/spaces/main.h"

using namespace StructBX::Functions::Spaces;

void MainData::ReadA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);

}

void MainData::ReadSpecificA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", get_space_id(), false);
}

void MainData::AddA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("SELECT s.id FROM spaces s WHERE s.identifier = ?");
    action_->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un espacio con este identificador ya existe");
            return false;
        }

        return true;
    });

    action_->AddParameter_("identifier", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void MainData::AddA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO spaces (identifier, name, description) VALUES (?, ?, ?)"
    );
    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador del espacio no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() <= 3)
        {
            param->set_error("El identificador debe tener más de 3 caracteres");
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
        if(param->ToString_() == "")
        {
            param->set_error("El nombre del espacio no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() <= 3)
        {
            param->set_error("El nombre del espacio debe tener más de 3 caracteres");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);
}

void MainData::AddA03::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "INSERT INTO spaces_users (id_naf_user, id_space) " \
        "SELECT ?, s.id FROM spaces s WHERE identifier = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("identifier", "", true);
}

void MainData::ChangeA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id, s.name, s.state, s.logo, s.description, s.created_at " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
    ->SetupCondition_("condition-id_space", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de cambio de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void MainData::ModifyA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-user-in-space", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El usuario actual no pertenece al espacio que intenta modificar");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void MainData::ModifyA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_final(false);
    action_->set_sql_code("SELECT id FROM spaces WHERE identifier = ? AND id != ?");
    action_->SetupCondition_("verify-space-identifier", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un espacio con este identificador ya existe");
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
}

void MainData::ModifyA03::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "SET s.identifier = ?, s.name = ?, s.description = ? " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );

    action_->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
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
        if(param->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action_->AddParameter_("description", "", true);
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });
}

void MainData::DeleteA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->SetupCondition_("verify-user-in-space", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El usuario actual no pertenece al espacio que intenta eliminar");
            return false;
        }

        return true;
    });

    action_->AddParameter_("id_naf_user", get_id_user(), false);
}

void MainData::DeleteA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "SET s.state = 'DELETED' " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );

    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

void MainData::DeleteA03::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code("DELETE FROM spaces_users WHERE id_space = ?");

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

Main::Main(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
    ,users_(function_data)
{
    Read_();
    ReadSpecific_();
    Change_();
    Add_();
    Modify_();
    Delete_();
}

void Main::Read_()
{
    // Function GET /api/spaces/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/read", HTTP::EnumMethods::kHTTP_GET);
    
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    actions_.read_a01_.Setup_(action);

    // Setup custom process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": " + action->get_custom_error());
            return;
        }

        // Iterate over results
        for(auto row : *action->get_results())
        {
            // Get form id
            auto id = row->ExtractField_("id");
            if(id->IsNull_())
                continue;

            // Action 2: Size
            auto action2 = NAF::Functions::Action("a2");
            action2.set_sql_code(
                "SELECT ROUND(SUM((DATA_LENGTH + INDEX_LENGTH)) / 1024 / 1024, 2) AS 'size' " \
                "FROM information_schema.TABLES " \
                "WHERE TABLE_SCHEMA = '_structbx_space_" + id->ToString_() + "'"
            );
            if(!action2.Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error KXkg8nuaNB");
                return;
            }

            // Size of space directory
            auto directory = NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded");
            directory += "/" + id->ToString_();
            float directory_size = 0;
            DirectoryIterator it(directory);
            DirectoryIterator end;
            while (it != end)
            {
                if(it->isDirectory())
                {
                    directory_size += it->getSize();
                    DirectoryIterator it2(it.path());
                    while (it2 != end)
                    {
                        directory_size += it2->getSize();
                        ++it2;
                    }
                }
                else
                {
                    directory_size += it->getSize();
                }

                ++it;
            }
            directory_size = directory_size / 1024.f / 1024.f;

            // Get results
            auto size = action2.get_results()->First_();
            if(size->IsNull_())
            {
                row->AddField_("size", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(0)));
                row->AddField_("directory_size", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(directory_size)));
            }
            else
            {
                row->AddField_("size", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(size->Float_())));
                row->AddField_("directory_size", NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(directory_size)));
            }

        }

        // JSON Results
        auto json_results = action->CreateJSONResult_();

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_results);
    });

    get_functions()->push_back(function);
}

void Main::ReadSpecific_()
{
    // Function GET /api/spaces/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    actions_.read_specific_a01_.Setup_(action);

    auto space_id = get_space_id();
    auto id_user = get_id_user();
    function->SetupCustomProcess_([space_id, id_user, action](NAF::Functions::Function& self)
    {
        // Get identifier
        auto identifier = self.GetParameter_("identifier");
        if(identifier != self.get_parameters().end())
        {
            action->get_parameters().clear();
            action->set_sql_code(
                "SELECT s.* " \
                "FROM spaces s " \
                "JOIN spaces_users su ON su.id_space = s.id " \
                "WHERE su.id_naf_user = ? AND s.identifier = ?"
            );
            action->AddParameter_("id_naf_user", id_user, false);
            action->AddParameter_("identifier", identifier->get()->ToString_(), true)
            ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
            {
                if(param->ToString_() == "")
                {
                    param->set_error("El identificador de espacio no puede estar vacío");
                    return false;
                }
                return true;
            });
        }

        if(!action->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": " + action->get_custom_error());
            return;
        }
        
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, action->get_json_result());
    });

    get_functions()->push_back(function);
}

void Main::Change_()
{
    // Function GET /api/spaces/change
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/change", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    actions_.change_a01_.Setup_(action);

    function->SetupCustomProcess_([&](NAF::Functions::Function& self)
    {
        // Search first action
        auto action = *self.get_actions().begin();
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "No actions found.");
            return;
        }
        
        // Execute action
        if(!action->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action->get_custom_error());
            return;
        }
        auto result = action->get_json_result();

        // Set Space ID Cookie to the client
        auto field = action->get_results()->First_();
        if(!field->IsNull_())
        {
            // Set Cookie Space ID
            auto space_id_encoded = NAF::Tools::Base64Tool().Encode_(field->ToString_());
            Net::HTTPCookie cookie(NAF::Tools::SettingsManager::GetSetting_("space_id_cookie_name", "1f3efd18688d2"), space_id_encoded);
            cookie.setPath("/");
            cookie.setSameSite(Net::HTTPCookie::SAME_SITE_STRICT);
            cookie.setSecure(true);
            cookie.setHttpOnly();

            auto& response = self.get_http_server_response().value();
            response->addCookie(cookie);
            
            // Send results
            self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);
        }
        else
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "El usuario no est&aacute; en alg&uacute;n espacio.");
    });

    get_functions()->push_back(function);
}

void Main::Add_()
{
    // Function GET /api/spaces/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action1: Verify space if exists
    auto action1 = function->AddAction_("a1");
    actions_.add_a01_.Setup_(action1);

    // Action2: Add space
    auto action2 = function->AddAction_("a2");
    actions_.add_a02_.Setup_(action2);

    // Action3: Add current user to the new space
    auto action3 = function->AddAction_("a3");
    actions_.add_a03_.Setup_(action3);

    // Action4: Create database
    auto action4 = function->AddAction_("a4");

    // Setup Custom Process
    function->SetupCustomProcess_([action1, action2, action3, action4](NAF::Functions::Function& self)
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

        // Get space ID
        auto space_id = action2->get_last_insert_id();
        if(space_id == 0)
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error bq0fyWtqeP");
            return;
        }

        // Create database
        action4->set_sql_code("CREATE DATABASE _structbx_space_" + std::to_string(space_id));
        if(!action4->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error " + action4->get_identifier() + ": No se pudo crear la DB de espacio");

            // Delete space from table
            NAF::Functions::Action action5("a5");
            action5.set_sql_code("DELETE FROM spaces WHERE id = ?");
            action5.AddParameter_("id", std::to_string(space_id), false);

            return;
        }

        // Create the directory to store files
        try
        {
            auto directory = NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded");
            directory += "/" + std::to_string(space_id);
            Poco::File file(directory);
            if(file.exists())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
                return;
            }
            if(!file.createDirectory())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos espacio");
                return;
            }

            // Send results
            self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
        }
        catch(Poco::FileException& e)
        {
            NAF::Tools::OutputLogger::Debug_(e.displayText());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos espacio");
            return;
        }
        catch(std::exception& e)
        {
            NAF::Tools::OutputLogger::Debug_(e.what());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos espacio");
            return;
        }
    });

    get_functions()->push_back(function);
}

void Main::Modify_()
{
    // Function PUT /api/spaces/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify that current user is in the space
    auto action1 = function->AddAction_("a1");
    actions_.modify_a01_.Setup_(action1);

    // Action 2: Verify space identifier
    auto action2 = function->AddAction_("a2");
    actions_.modify_a02_.Setup_(action2);

    // Action 3: Modify space
    auto action3 = function->AddAction_("a3");
    actions_.modify_a03_.Setup_(action3);

    get_functions()->push_back(function);
}

void Main::Delete_()
{
    // Function GET /api/spaces/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/delete", HTTP::EnumMethods::kHTTP_DEL);

    // Action 1: Verify that current user is in the space
    auto action1 = function->AddAction_("a1");
    actions_.delete_a01_.Setup_(action1);

    // Action 2: Mark space like "deleted"
    auto action2 = function->AddAction_("a2");
    actions_.delete_a02_.Setup_(action2);

    // Action 3: Delete users from space
    auto action3 = function->AddAction_("a3");
    actions_.delete_a03_.Setup_(action3);

    get_functions()->push_back(function);
}
