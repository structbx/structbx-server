
#include "functions/general/users.h"

using namespace StructBX::Functions::General;

Users::Users(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_current_(function_data)
    ,struct_read_specific_(function_data)
    ,struct_modify_current_username_(function_data)
    ,struct_modify_current_password_(function_data)
    ,struct_add_(function_data)
    ,struct_modify_(function_data)
    ,struct_delete_(function_data)
{
    
}

Users::Read::Read(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.id, nu.username, nu.status, nu.id_group, nu.created_at, ng.group AS 'group' "
        "FROM users nu "
        "JOIN groups ng ON ng.id = nu.id_group "
    );

    get_functions()->push_back(function);
}

Users::ReadCurrent::ReadCurrent(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/current/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/current/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.id, nu.username, nu.status, nu.id_group, nu.created_at, ng.group AS 'group' "
        "FROM users nu "
        "JOIN groups ng ON ng.id = nu.id_group "
        "WHERE nu.id = ?"
    );
    action1->AddParameter_("id_user", get_id_user(), false);

    get_functions()->push_back(function);
}

Users::ReadSpecific::ReadSpecific(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/read/id", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.id, nu.username, nu.status, nu.id_group, nu.created_at, ng.group AS 'group' "
        "FROM users nu "
        "JOIN groups ng ON ng.id = nu.id_group "
        "WHERE nu.id = ?"
    );
    action1->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id_user", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de usuario no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}

Users::ModifyCurrentUsername::ModifyCurrentUsername(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/current/username/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/current/username/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    // Action1: Verify if username don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action2: Modify username
    auto action2 = function->AddAction_("a2");
    A2(action2);

    get_functions()->push_back(function);
}

void Users::ModifyCurrentUsername::A1(NAF::Functions::Action::Ptr action)
{
    action->set_final(false);
    action->set_sql_code("SELECT id FROM users WHERE username = ? AND id != ?");
    action->SetupCondition_("verify-username-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este nombre de usuario ya est&aacute; registrado");
            return false;
        }

        return true;
    });

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id", get_id_user(), false);
}

void Users::ModifyCurrentUsername::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE users "
        "SET username = ? "
        "WHERE id = ?"
    );

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_naf_user", get_id_user(), false);
}

Users::ModifyCurrentPassword::ModifyCurrentPassword(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/current/password/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/current/password/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action1: Verify current password
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action2: Save new password
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2](NAF::Functions::Function& self)
    {
        auto new_password = self.GetParameter_("new_password");
        auto new_password2 = self.GetParameter_("new_password2");
        if(new_password == self.get_parameters().end() || new_password2 == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Hubo un error al modificar la contraseña");
            return;
        }
        // Verify passwords is same
        if(new_password->get()->ToString_() != new_password2->get()->ToString_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Las contraseñas no coinciden");
            return;
        }

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

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok");
    });

    get_functions()->push_back(function);
}

void Users::ModifyCurrentPassword::A1(NAF::Functions::Action::Ptr action)
{
    action->set_final(false);
    action->set_sql_code("SELECT id FROM users WHERE password = ? AND id = ?");
    action->SetupCondition_("verify-username-password", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("La contrase&ntilde;a actual es incorrecta");
            return false;
        }

        return true;
    });

    action->AddParameter_("current_password", "", true)
    ->SetupCondition_("condition-current_password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("La contrase&ntilde;a actual no puede estar vacía");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));

        return true;
    });
    action->AddParameter_("id", get_id_user(), false);
}

void Users::ModifyCurrentPassword::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE users "
        "SET password = ? "
        "WHERE id = ?"
    );

    action->AddParameter_("new_password", "", true)
    ->SetupCondition_("condition-password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("La contraseña no puede estar vacía");
            return false;
        }
        if(param->ToString_().size() < 8)
        {
            param->set_error("La contraseña no puede ser menor a 8 dígitos");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
        return true;
    });
    action->AddParameter_("id_naf_user", get_id_user(), false);
}

Users::Add::Add(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/add", HTTP::EnumMethods::kHTTP_POST);
    
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action1: Verify if username don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action2: Add username
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Setup custom process
    function->SetupCustomProcess_([action1, action2](NAF::Functions::Function& self)
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

        // New User ID
        auto user_id = action2->get_last_insert_id();
        if(user_id < 1)
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Hubo un error al guardar el usuario");
            return;
        }
        
        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok");
    });
    get_functions()->push_back(function);
}

void Users::Add::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("SELECT id FROM users WHERE username = ?");
    action->SetupCondition_("verify-username-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este nombre de usuario ya est&aacute; registrado");
            return false;
        }

        return true;
    });

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void Users::Add::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "INSERT INTO users (username, password, status, id_group) "
        "VALUES (?, ?, ?, ?) "
    );

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action->AddParameter_("password", "", true)
    ->SetupCondition_("condition-password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("La contraseña no puede estar vacía");
            return false;
        }
        if(param->ToString_().size() < 8)
        {
            param->set_error("La contraseña no puede ser menor a 8 dígitos");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
        return true;
    });
    action->AddParameter_("status", "activo", false);
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

Users::Modify::Modify(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action1: Verify if username don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action2: Modify user (with password)
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action2: Modify user (without password)
    auto action3 = function->AddAction_("a3");
    A3(action3);

    // Setup custom process
    function->SetupCustomProcess_([action1, action2, action3](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }

        // Password parameter
        auto password = self.GetParameter_("password");
        if(password == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error uukK3aINI5");
            return;
        }

        // Verify if password is not empty
        if(password->get()->ToString_() != "")
        {
            if(!action2->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
                return;
            }
        }
        else
        {
            if(!action3->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action3->get_identifier() + ": " + action3->get_custom_error());
                return;
            }
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok");
    });

    get_functions()->push_back(function);
}

void Users::Modify::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("SELECT id FROM users WHERE username = ? AND id != ?");
    action->SetupCondition_("verify-username-existence", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este nombre de usuario ya est&aacute; registrado");
            return false;
        }

        return true;
    });

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void Users::Modify::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE users n "
        "SET username = ?, password = ?, status = ?, id_group = ? "
        "WHERE n.id = ? "
    );

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action->AddParameter_("password", "", true)
    ->SetupCondition_("condition-password", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("La contraseña no puede estar vacía");
            return false;
        }
        if(param->ToString_().size() < 8)
        {
            param->set_error("La contraseña no puede ser menor a 8 dígitos");
            return false;
        }

        std::string password = param->ToString_();
        std::string password_encoded = NAF::Tools::HMACTool().Encode_(password);
        param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(password_encoded)));
        return true;
    });
    action->AddParameter_("status", "", true)
    ->SetupCondition_("condition-status", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id del usuario no puede estar vacío");
            return false;
        }
        return true;
    });
}

void Users::Modify::A3(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "UPDATE users n "
        "SET username = ?, status = ?, id_group = ? "
        "WHERE n.id = ? "
    );

    action->AddParameter_("username", "", true)
    ->SetupCondition_("condition-username", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
        {
            param->set_error("El nombre de usuario debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre de usuario no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre de usuario no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = Tools::IDChecker().CheckEmail_(param->get_value()->ToString_());
        if(!result)
        {
            param->set_error("El nombre de usuario solo puede tener a-z, A-Z, 0-9, \"_\", \".\", \"@\", sin espacios en blanco");
            return false;
        }
        return true;
    });
    action->AddParameter_("status", "", true)
    ->SetupCondition_("condition-status", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id del usuario no puede estar vacío");
            return false;
        }
        return true;
    });
}

Users::Delete::Delete(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
{
    // Function GET /api/general/users/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/users/delete", HTTP::EnumMethods::kHTTP_DEL);
    
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Users::Delete::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "DELETE nu FROM users nu "
        "WHERE "
            "nu.id = ? "
    );
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_user", get_id_user(), false);
}