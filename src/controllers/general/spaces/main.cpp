
#include "controllers/general/spaces/main.h"

using namespace StructBX::Controllers::General::Spaces;

Main::Main(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,function_users_(function_data)
    ,struct_read_(function_data)
    ,struct_read_specific_(function_data)
    ,struct_change_(function_data)
    ,struct_add_(function_data)
    ,struct_modify_(function_data)
    ,struct_delete_(function_data)
{
    
}

Main::Read::Read(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/spaces/read
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/spaces/read", HTTP::EnumMethods::kHTTP_GET);
    
    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    A1(action);

    // Setup custom process
    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id, action](StructBX::Functions::Function& self)
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
            auto action2 = StructBX::Functions::Action("a2");
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
            auto directory = StructBX::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded");
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
                row->AddField_("size", StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(0)));
                row->AddField_("directory_size", StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(directory_size)));
            }
            else
            {
                row->AddField_("size", StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(size->Float_())));
                row->AddField_("directory_size", StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(directory_size)));
            }

        }

        // JSON Results
        auto json_results = action->CreateJSONResult_();

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_results);
    });

    get_functions()->push_back(function);
}

void Main::Read::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);
}

Main::ReadSpecific::ReadSpecific(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/spaces/read/id
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/spaces/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    A1(action);

    auto space_id = get_space_id();
    auto id_user = get_id_user();
    function->SetupCustomProcess_([space_id, id_user, action](StructBX::Functions::Function& self)
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

void Main::ReadSpecific::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("id_space", get_space_id(), false);
}

Main::Add::Add(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/spaces/add
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/spaces/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action1: Verify space if exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action2: Add space
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action3: Add current user to the new space
    auto action3 = function->AddAction_("a3");
    A3(action3);

    // Action4: Create database
    auto action4 = function->AddAction_("a4");

    // Setup Custom Process
    function->SetupCustomProcess_([action1, action2, action3, action4](StructBX::Functions::Function& self)
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
            StructBX::Functions::Action action5("a5");
            action5.set_sql_code("DELETE FROM spaces WHERE id = ?");
            action5.AddParameter_("id", std::to_string(space_id), false);

            return;
        }

        // Create the directory to store files
        try
        {
            auto directory = StructBX::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded");
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
            StructBX::Tools::OutputLogger::Debug_(e.displayText());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos espacio");
            return;
        }
        catch(std::exception& e)
        {
            StructBX::Tools::OutputLogger::Debug_(e.what());
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error: No se pudo crear el directorio de archivos espacio");
            return;
        }
    });

    get_functions()->push_back(function);
}

void Main::Add::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code("SELECT s.id FROM spaces s WHERE s.identifier = ?");
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un espacio con este identificador ya existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("identifier", StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue()), true)
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

void Main::Add::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "INSERT INTO spaces (identifier, name, description) VALUES (?, ?, ?)"
    );
    action->AddParameter_("identifier", "", true)
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
    action->AddParameter_("name", "", true)
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
    action->AddParameter_("description", "", true);
}

void Main::Add::A3(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "INSERT INTO spaces_users (id_naf_user, id_space) " \
        "SELECT ?, s.id FROM spaces s WHERE identifier = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("identifier", "", true);
}

Main::Change::Change(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/spaces/change
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/spaces/change", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    A1(action);

    function->SetupCustomProcess_([&](StructBX::Functions::Function& self)
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
            auto space_id_encoded = StructBX::Tools::Base64Tool().Encode_(field->ToString_());
            Net::HTTPCookie cookie(StructBX::Tools::SettingsManager::GetSetting_("space_id_cookie_name", "1f3efd18688d2"), space_id_encoded);
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

void Main::Change::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT s.id, s.name, s.state, s.logo, s.description, s.created_at " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("id_space", StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue()), true)
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

Main::Modify::Modify(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function PUT /api/spaces/modify
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/spaces/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify that current user is in the space
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2: Verify space identifier
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Modify space
    auto action3 = function->AddAction_("a3");
    A3(action3);

    get_functions()->push_back(function);
}

void Main::Modify::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT s.id " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action->SetupCondition_("verify-user-in-space", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El usuario actual no pertenece al espacio que intenta modificar");
            return false;
        }

        return true;
    });

    action->AddParameter_("id_naf_user", get_id_user(), false);
}

void Main::Modify::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_final(false);
    action->set_sql_code("SELECT id FROM spaces WHERE identifier = ? AND id != ?");
    action->SetupCondition_("verify-space-identifier", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un espacio con este identificador ya existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id", "", true)
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

void Main::Modify::A3(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "SET s.identifier = ?, s.name = ?, s.description = ? " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );

    action->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(StructBX::Tools::DValue::Type::kString))
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
    action->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(StructBX::Tools::DValue::Type::kString))
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
    action->AddParameter_("description", "", true);
    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("id", "", true)
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

Main::Delete::Delete(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/spaces/delete
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/spaces/delete", HTTP::EnumMethods::kHTTP_DEL);

    // Action 1: Verify that current user is in the space
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2: Mark space like "deleted"
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Delete users from space
    auto action3 = function->AddAction_("a3");
    A3(action3);

    get_functions()->push_back(function);
}

void Main::Delete::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT s.id " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action->SetupCondition_("verify-user-in-space", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El usuario actual no pertenece al espacio que intenta eliminar");
            return false;
        }

        return true;
    });

    action->AddParameter_("id_naf_user", get_id_user(), false);
}

void Main::Delete::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "SET s.state = 'DELETED' " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );

    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("id", "", true)
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

void Main::Delete::A3(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code("DELETE FROM spaces_users WHERE id_space = ?");

    action->AddParameter_("id", "", true)
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
