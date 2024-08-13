
#include "functions/organizations.h"

Organizations::Organizations(std::string username) :
    username_(username)
{
    Read_();
    ReadSpecific_();
    ReadLogo_();
    Add_();
    Modify_();
    ModifyLogo_();
    Delete_();
}

void Organizations::Read_()
{

}

void Organizations::ReadSpecific_()
{
    // Function GET /api/organization/general/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/general/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT co.id AS id, co.name AS name, co.state AS state, co.description AS description, co.created_at AS created_at " \
        "FROM cloud_organizations co " \
        "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
        "WHERE cu.email = ?"
    );

    // Parameters and conditions
    action->AddParameter_("email", Tools::DValue(username_), false);

    functions_.push_back(function);
}

void Organizations::ReadLogo_()
{
    // Function GET /api/organization/logo/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/logo/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(Functions::Function::ResponseType::kFile);
    function->get_file_manager()->set_directory_base("/srv/www/app1.structbi.com-uploaded");
    function->get_file_manager()->AddBasicSupportedFiles_();

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT co.logo AS logo " \
        "FROM cloud_organizations co " \
        "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
        "WHERE cu.email = ?"
    );
    action->SetupCondition_("condition-action", Query::ConditionType::kWarning, [](Functions::Action& self)
    {
        auto first = self.get_results()->First_();
        if(first->IsNull_())
        {
            auto row = self.get_results()->AddRow_();
            row->AddField_("logo", Tools::DValue("noimage.png"));
        }
        else if(first->get_value().TypeIsIqual_(Tools::DValue::Type::kEmpty))
        {
            first->set_value(Tools::DValue("noimage.png"));
        }
        
        return true;
    });

    // Parameters and conditions
    action->AddParameter_("email", Tools::DValue(username_), false);

    functions_.push_back(function);
}

void Organizations::Add_()
{

}

void Organizations::Modify_()
{
    // Function PUT /api/organization/general/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/general/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Modify organization
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "UPDATE cloud_organizations co " \
        "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
        "SET co.name = ?, co.description = ? " \
        "WHERE cu.email = ?"
    );

    // Parameters and conditions
    action1->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
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
    action1->AddParameter_("description", Tools::DValue(""), true);

    action1->AddParameter_("email", Tools::DValue(username_), false);

    functions_.push_back(function);
}

void Organizations::ModifyLogo_()
{
    // Function PUT /api/organization/logo/modify/
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/logo/modify", HTTP::EnumMethods::kHTTP_PUT);
    function->set_response_type(Functions::Function::ResponseType::kCustom);
    function->get_file_manager()->set_directory_base("/srv/www/app1.structbi.com-uploaded");
    function->get_file_manager()->AddSupportedFile_("png", Files::FileProperties{"image/png", true, {""}});

    function->SetupCustomProcess_([](Functions::Function& self)
    {
        // Request logo path in DB
        Functions::Action a1("a1");
        a1.set_sql_code(
            "SELECT co.logo AS logo " \
            "FROM cloud_organizations co " \
            "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
            "WHERE cu.email = ?"
        );
        a1.AddParameter_("email", Tools::DValue(self.get_current_user().get_username()), false);

        a1.ComposeQuery_();
        a1.ExecuteQuery_();
        a1.MakeResults_();

        auto field = a1.get_results()->First_();
        if(!field->IsNull_())
        {
            if(!field->get_value().TypeIsIqual_(Tools::DValue::Type::kEmpty))
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
            "UPDATE cloud_organizations co " \
            "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
            "SET co.logo = ? " \
            "WHERE cu.email = ?"
        );
        
        a2.AddParameter_("logo", Tools::DValue(front_file.get_requested_path()->getFileName()), false);
        auto user = Tools::DValue(self.get_current_user().get_username());
        a2.AddParameter_("email", user, false);

        a2.ComposeQuery_();
        a2.ExecuteQuery_();

        // Response
        self.CompoundFillResponse_(HTTP::Status::kHTTP_OK, self.get_file_manager()->get_result(), "Ok.");
    });

    functions_.push_back(function);
}

void Organizations::Delete_()
{
    
}