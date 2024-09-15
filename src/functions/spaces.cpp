
#include "functions/spaces.h"

Spaces::Spaces(int id_user) :
    id_user_(id_user)
{
    Read_();
    ReadSpecific_();
    ReadLogo_();
    Add_();
    Modify_();
    ModifyLogo_();
    Delete_();
}

void Spaces::Read_()
{

}

void Spaces::ReadSpecific_()
{
    // Function GET /api/spaces/general/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/general/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT o.id AS id, o.name AS name, o.state AS state, o.description AS description, o.created_at AS created_at " \
        "FROM spaces o " \
        "JOIN spaces_users ou ON ou.id_space = o.id " \
        "WHERE ou.id_naf_user = ?"
    );

    // Parameters and conditions
    action->AddParameter_("id_naf_user", Tools::DValue(id_user_), false);

    functions_.push_back(function);
}

void Spaces::ReadLogo_()
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
            row->AddField_("logo", Tools::DValue("noimage.png"));
        }
        else if(first->get_value().TypeIsIqual_(Tools::DValue::Type::kEmpty))
        {
            first->set_value(Tools::DValue("noimage.png"));
        }
        
        return true;
    });

    // Parameters and conditions
    action->AddParameter_("id_naf_user", Tools::DValue(id_user_), false);

    functions_.push_back(function);
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

    action1->AddParameter_("id_naf_user", Tools::DValue(id_user_), false);

    functions_.push_back(function);
}

void Spaces::ModifyLogo_()
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
        a1.AddParameter_("id_naf_user", Tools::DValue(self.get_current_user().get_id()), false);

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
            "UPDATE spaces o " \
            "JOIN spaces_users ou ON ou.id_space = o.id " \
            "SET o.logo = ? " \
            "WHERE ou.id_naf_user = ?"
        );
        
        a2.AddParameter_("logo", Tools::DValue(front_file.get_requested_path()->getFileName()), false);
        auto user = Tools::DValue(self.get_current_user().get_id());
        a2.AddParameter_("id_naf_user", user, false);

        a2.ComposeQuery_();
        a2.ExecuteQuery_();

        // Response
        self.CompoundFillResponse_(HTTP::Status::kHTTP_OK, self.get_file_manager()->get_result(), "Ok.");
    });

    functions_.push_back(function);
}

void Spaces::Delete_()
{
    
}