
#include "functions/spaces.h"

SpacesLogo::SpacesLogo(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    Modify_();
}

void SpacesLogo::Read_()
{
    // Function GET /api/spaces/logo/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/logo/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(Functions::Function::ResponseType::kFile);
    function->get_file_manager()->set_directory_base("/srv/www/app1.structbi.com-uploaded");
    function->get_file_manager()->AddBasicSupportedFiles_();

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT o.logo AS logo " \
        "FROM spaces o " \
        "JOIN spaces_users ou ON ou.id_space = o.id " \
        "WHERE ou.id_naf_user = ?"
    );
    action->SetupCondition_("condition-action", Query::ConditionType::kWarning, [](Functions::Action& self)
    {
        auto first = self.get_results()->First_();
        if(first->IsNull_())
        {
            auto row = self.get_results()->AddRow_();
            row->AddField_("logo", Tools::DValue::Ptr(new Tools::DValue("noimage.png")));
        }
        else if(first->get_value()->TypeIsIqual_(Tools::DValue::Type::kEmpty))
        {
            first->set_value(Tools::DValue::Ptr(new Tools::DValue("noimage.png")));
        }
        
        return true;
    });

    // Parameters and conditions
    action->AddParameter_("id_naf_user", get_id_user(), false);

    get_functions()->push_back(function);
}

void SpacesLogo::Modify_()
{
    // Function PUT /api/spaces/logo/modify/
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/logo/modify", HTTP::EnumMethods::kHTTP_PUT);
    function->set_response_type(Functions::Function::ResponseType::kCustom);
    function->get_file_manager()->set_directory_base("/srv/www/app1.structbi.com-uploaded");
    function->get_file_manager()->AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});

    function->SetupCustomProcess_([](Functions::Function& self)
    {
        // Request logo path in DB
        Functions::Action a1("a1");
        a1.set_sql_code(
            "SELECT o.logo AS logo " \
            "FROM spaces co " \
            "JOIN spaces_users ou ON ou.id_space = o.id " \
            "WHERE ou.id_naf_user = ?"
        );
        a1.AddParameter_("id_naf_user", self.get_current_user().get_id(), false);

        a1.ComposeQuery_();
        a1.ExecuteQuery_();
        a1.MakeResults_();

        auto field = a1.get_results()->First_();
        if(!field->IsNull_())
        {
            if(!field->get_value()->TypeIsIqual_(Tools::DValue::Type::kEmpty))
            {
                std::string logo_path = field->ToString_();

                // Verify logo exists and remove it
                Files::FileManager file_manager;
                file_manager.set_directory_base("/srv/www/app1.structbi.com-uploaded");
                file_manager.set_operation_type(Files::OperationType::kDelete);
                file_manager.get_files().push_back(file_manager.CreateTempFile_("/" + logo_path));

                if(file_manager.CheckFiles_())
                {
                    file_manager.RemoveFile_();
                    return;
                }
            }
        }

        // Upload a new logo
        self.get_file_manager()->set_operation_type(Files::OperationType::kUpload);
        auto& front_file = self.get_file_manager()->get_files().front();
        
        if(!self.get_file_manager()->ChangePathAndFilename_(front_file, self.get_file_manager()->get_directory_base()))
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error al subir el archivo.");
            return;
        }
        if(!self.get_file_manager()->IsSupported_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Archivo no soportado, debe ser formato PNG.");
            return;
        }
        if(!self.get_file_manager()->VerifyMaxFileSize_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "El archivo debe ser de menos de 15MB.");
            return;
        }
        self.get_file_manager()->UploadFile_();

        // Save path in DB
        Functions::Action a2("a2");
        a2.set_sql_code(
            "UPDATE spaces o " \
            "JOIN spaces_users ou ON ou.id_space = o.id " \
            "SET o.logo = ? " \
            "WHERE ou.id_naf_user = ?"
        );
        
        a2.AddParameter_("logo", front_file.get_requested_path()->getFileName(), false);
        a2.AddParameter_("id_naf_user", self.get_current_user().get_id(), false);

        a2.ComposeQuery_();
        a2.ExecuteQuery_();

        // Response
        self.CompoundFillResponse_(HTTP::Status::kHTTP_OK, self.get_file_manager()->get_result(), "Ok.");
    });

    get_functions()->push_back(function);
}

SpacesUsers::SpacesUsers(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
}

void SpacesUsers::Read_()
{
    // Function GET /api/spaces/users/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/users/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT nu.username, nu.status, nu.created_at, ng.id AS id_group, ng.`group` AS 'group' " \
        "FROM _naf_users nu " \
        "JOIN _naf_groups ng ON ng.id = nu.id_group"
    );

    get_functions()->push_back(function);
}

Spaces::Spaces(FunctionData& function_data) :
    FunctionData(function_data)
    ,logo_(function_data)
    ,users_(function_data)
{
    Read_();
    ReadSpecific_();
    Change_();
    Add_();
    Modify_();
    Delete_();
}

void Spaces::Read_()
{
    // Function GET /api/spaces/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);

    get_functions()->push_back(function);
}

void Spaces::ReadSpecific_()
{
    // Function GET /api/spaces/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(Functions::Function::ResponseType::kCustom);

    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](Functions::Function& self)
    {
        // Function to request the current Space
        Functions::Action action("a1");

        if(space_id == "")
        {
            // Request space ID in DB (Not found in cookie)
            action.set_sql_code(
                "SELECT s.* " \
                "FROM spaces s " \
                "JOIN spaces_users su ON su.id_space = s.id " \
                "WHERE su.id_naf_user = ?"
            );
            action.AddParameter_("id_naf_user", self.get_current_user().get_id(), false);
        }
        else
        {
            // Request space ID in Cookie
            action.set_sql_code(
                "SELECT s.* " \
                "FROM spaces s " \
                "JOIN spaces_users su ON su.id_space = s.id " \
                "WHERE su.id_naf_user = ? AND s.id = ?"
            );
            action.AddParameter_("id_naf_user", self.get_current_user().get_id(), false);
            action.AddParameter_("id_space", space_id, false);
        }

        // Execute action
        if(!action.Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action.get_custom_error());
            return;
        }
        auto result = action.get_json_result();

        // Set Space ID Cookie to the client
        auto field = action.get_results()->First_();
        if(!field->IsNull_())
        {
            if(space_id == "")
            {
                // If space is empty, set the cookie
                Net::HTTPCookie cookie("structbi-space-id", field->ToString_());
                cookie.setPath("/");

                auto& response = self.get_http_server_response().value();
                response->addCookie(cookie);
            }

            self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);
        }
        else
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "El usuario no est&aacute; en alg&uacute;n espacio.");
    });

    get_functions()->push_back(function);
}

void Spaces::Change_()
{
    // Function GET /api/spaces/change
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/change", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT s.id, s.name, s.state, s.logo, s.description, s.created_at " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("id_space", Tools::DValue::Ptr(new Tools::DValue()), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de nuevo espacio no puede estar vacío");
            return false;
        }
        return true;
    });

    function->SetupCustomProcess_([&](Functions::Function& self)
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
            Net::HTTPCookie cookie("structbi-space-id", field->ToString_());
            cookie.setPath("/");

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

void Spaces::Add_()
{

}

void Spaces::Modify_()
{
    // Function PUT /api/spaces/general/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/general/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Modify space
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "UPDATE spaces o " \
        "JOIN spaces_users ou ON ou.id_space = o.id " \
        "SET o.name = ?, o.description = ? " \
        "WHERE ou.id_naf_user = ?"
    );

    // Parameters and conditions
    action1->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
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
    action1->AddParameter_("description", "", true);

    action1->AddParameter_("id_naf_user", get_id_user(), false);

    get_functions()->push_back(function);
}

void Spaces::Delete_()
{
    
}
