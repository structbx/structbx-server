
#include "functions/spaces/spaces_logo.h"

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
    function->get_file_manager()->set_directory_base(Tools::SettingsManager::GetSetting_("directory_base", "/var/www/app1.structbi.com-uploaded"));
    function->get_file_manager()->AddBasicSupportedFiles_();

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT s.logo AS logo " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
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
    function->get_file_manager()->set_directory_base(Tools::SettingsManager::GetSetting_("directory_base", "/var/www/app1.structbi.com-uploaded"));
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

        // Execute action
        if(!a1.Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + a1.get_identifier() + ": D2vnoHsyoi");
            return;
        }
        
        auto field = a1.get_results()->First_();
        if(!field->IsNull_())
        {
            if(!field->get_value()->TypeIsIqual_(Tools::DValue::Type::kEmpty))
            {
                std::string logo_path = field->ToString_();

                // Verify logo exists and remove it
                Files::FileManager file_manager;
                file_manager.set_directory_base(Tools::SettingsManager::GetSetting_("directory_base", "/var/www/app1.structbi.com-uploaded"));
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

        // Execute action
        if(!a1.Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + a1.get_identifier() + ": KcWQyCZCqT");
            return;
        }
        
        // Response
        self.CompoundFillResponse_(HTTP::Status::kHTTP_OK, self.get_file_manager()->get_result(), "Ok.");
    });

    get_functions()->push_back(function);
}
