
#include "functions/forms/forms_data.h"
#include <tools/dvalue.h>

StructBI::Functions::FormsData::FormsData(Tools::FunctionData& function_data) :
    FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    ReadSpecific_();
    ReadFile_();
    Add_();
    Modify_();
    Delete_();
}

void StructBI::Functions::FormsData::Read_()
{
    // Function GET /api/forms/data/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/data/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1_0: Get form id
    auto action1_0 = function->AddAction_("a1_0");
    actions_.forms_data_.read_a01_0_.Setup_(action1_0);

    // Action 1: Get form columns
    auto action1 = function->AddAction_("a1");
    actions_.forms_data_.read_a01_.Setup_(action1);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1_0, action1](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1_0->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1_0->get_identifier() + ": " + action1_0->get_custom_error());
            return;
        }
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }

        // Get form ID
        auto form_id = action1_0->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error EqLiZk1pm7");
            return;
        }

        // Get columns
        std::string columns = "";
        std::string joins = "";
        bool has_link = false;
        for(auto it : *action1->get_results())
        {
            Query::Field::Ptr id = it.get()->ExtractField_("id");
            Query::Field::Ptr name = it.get()->ExtractField_("name");
            Query::Field::Ptr link_to = it.get()->ExtractField_("link_to");
            if(id->IsNull_() || name->IsNull_())
                continue;

            std::string column = "_structbi_column_" + id->ToString_() + " AS '" + name->ToString_() + "'";

            // Get link columns
            if(!link_to->IsNull_())
            {
                has_link = true;

                // Get table columns (link)
                auto action1_2 = self.AddAction_("a1_2");
                action1_2->set_sql_code("SELECT * FROM forms_columns WHERE id_form = ?");
                action1_2->AddParameter_("id", link_to->Int_(), false);
                if(!action1_2->Work_())
                {
                    self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error wmAdRBELoO");
                    return;
                }
                if(action1_2->get_results()->size() < 2)
                {
                    self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error OFYV54ToXi");
                    return;
                }

                // Get ID from first and second column
                auto column2 = action1_2->get_results()->begin();
                auto column2_id = column2->get()->ExtractField_("id");
                column2++;
                auto column2_visualization = column2->get()->ExtractField_("id");
                if(column2_id->IsNull_())
                {
                    self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error NmYoq56SDN");
                    return;
                }

                // Setup column link
                column = "_" + link_to->ToString_() + "._structbi_column_" + column2_visualization->ToString_() + " AS '" + name->ToString_() + "'";

                // Setup new join
                joins += " LEFT JOIN _structbi_space_" + id_space + "._structbi_form_" + link_to->ToString_() +
                " AS _" + link_to->ToString_() + " ON _" + link_to->ToString_() + "._structbi_column_" + column2_id->ToString_() + 
                " = _" + form_id->ToString_() + "._structbi_column_" + id->ToString_();
            }

            // Set column
            if(it == *action1->get_results()->begin())
                columns = column;
            else
                columns += ", " + column;
        }

        // Verify if columns is empty
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "No existen columnas en la tabla");
            return;
        }

        // Action 2: Get Form data
        auto action2 = self.AddAction_("a2");
        std::string sql_code = 
            "SELECT " + columns + " " \
            "FROM _structbi_space_" + id_space + "._structbi_form_" + form_id->ToString_() + 
                " AS _" + form_id->ToString_()
        ;

        // Prepare JOIN if there is a link
        if(has_link)
            sql_code += joins;

        // Execute
        action2->set_sql_code(sql_code);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error UgOMMObhM2");
            return;
        }

        // Results
        auto json_result1 = action1->get_json_result();
        auto json_result2 = action2->get_json_result();
        json_result2->set("status", action2->get_status());
        json_result2->set("message", action2->get_message());
        json_result2->set("columns_meta", json_result1);

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_result2);
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsData::ReadSpecific_()
{
    // Function GET /api/forms/data/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/data/read/id", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1_0: Get form id
    auto action1_0 = function->AddAction_("a1_0");
    actions_.forms_data_.read_a01_0_.Setup_(action1_0);

    // Action 1: Get form columns
    auto action1 = function->AddAction_("a1");
    actions_.forms_data_.read_specific_a01_.Setup_(action1);

    // Action 2: Get Form data
    auto action2 = function->AddAction_("a2");
    actions_.forms_data_.read_specific_a02_.Setup_(action2);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1_0,action1, action2](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1_0->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1_0->get_identifier() + ": " + action1_0->get_custom_error());
            return;
        }
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }

        // Get form ID
        auto form_id = action1_0->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error HPqWlZkEbk");
            return;
        }

        // Get Column ID
        auto column_id = action1_0->get_results()->front()->ExtractField_("column_id");
        if(column_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 7UL6KqIryh");
            return;
        }

        // Get columns
        std::string columns = "";
        for(auto it : *action1->get_results())
        {
            Query::Field::Ptr id = it.get()->ExtractField_("id");
            Query::Field::Ptr name = it.get()->ExtractField_("name");
            if(id->IsNull_() || name->IsNull_())
                continue;

            if(columns == "")
                columns = "_structbi_column_" + id->ToString_() + " AS '" + name->ToString_() + "'";
            else
                columns += ", _structbi_column_" + id->ToString_() + " AS '" + name->ToString_() + "'";
        }

        // Verify if columns is empty
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "No existen columnas en la tabla");
            return;
        }

        // Action 2: Get Form data
        action2->set_sql_code(
            "SELECT " + columns + " " \
            "FROM _structbi_space_" + id_space + "._structbi_form_" + form_id->ToString_() + " " \
            "WHERE _structbi_column_" + column_id->ToString_() + " = ?");

        // Identify parameters and work
        self.IdentifyParameters_(action2);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error 3FqSnoQ4ru");
            return;
        }

        // Results
        auto json_result1 = action1->get_json_result();
        auto json_result2 = action2->get_json_result();
        json_result2->set("status", action2->get_status());
        json_result2->set("message", action2->get_message());
        json_result2->set("columns_meta", json_result1);

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_result2);
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsData::ReadFile_()
{
    // Function GET /api/forms/data/file/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/data/file/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](NAF::Functions::Function& self)
    {
        // Setup file manager
        self.get_file_manager()->AddBasicSupportedFiles_();
        self.get_file_manager()->set_directory_base(
            NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbi-web-uploaded") + "/" + std::string(id_space)
        );

        // Get filepath
        auto filepath = self.GetParameter_("filepath");
        if(filepath == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "El nombre del archivo no puede estar vacío");
            return;
        }

        // Download process
        auto string_path = filepath->get()->ToString_();
        self.DownloadProcess_(string_path);
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsData::Add_()
{
    // Function GET /api/forms/data/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/data/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_data_.add_01_.Setup_(action1);

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    actions_.forms_data_.add_02_.Setup_(action2);

    // Action 3: Save new record
    auto action3 = function->AddAction_("a3");
    actions_.forms_data_.add_03_.Setup_(action3);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2, action3](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": nrjlOllSqm");
            return;
        }
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": 9e8LhYKOdu");
            return;
        }

        // Get form ID
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error PPM2dLq5wk");
            return;
        }

        // Iterate over action2 results
        std::string columns = "";
        std::string values = "";
        for(auto it : *action2->get_results())
        {
            // Get column
            auto id = it.get()->ExtractField_("id");
            auto identifier = it.get()->ExtractField_("identifier");
            auto column_type = it.get()->ExtractField_("column_type");
            auto name = it.get()->ExtractField_("name");
            auto length = it.get()->ExtractField_("length");
            auto required = it.get()->ExtractField_("required");
            auto default_value = it.get()->ExtractField_("default_value");

            // Verify identifier is not null
            if(identifier->IsNull_())
                continue;

            // Verify identifier is not the id
            if(identifier->ToString_() == "id")
                continue;

            // Setup columns and values string
            if(columns == "")
            {
                columns = "_structbi_column_" + id->ToString_();
                values = "?";
            }
            else
            {
                columns += ",_structbi_column_" + id->ToString_();
                values += ", ?";
            }

            // Get file manager
            auto file_manager = self.get_file_manager();
            file_manager->set_directory_base(
                NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbi-web-uploaded") + "/" + std::string(id_space)
            );

            // Setup parameter
            action3->AddParameter_(identifier->ToString_(), NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
            ->SetupCondition_(identifier->ToString_(), Query::ConditionType::kError, [file_manager, length, required, default_value, column_type](Query::Parameter::Ptr param)
            {
                FileProcessing fp;
                fp.file_manager = file_manager;
                fp.param = param;
                fp.modify = false;
                fp.filepath = "";
                ParameterVerification pv(length, required, default_value, column_type, fp);
                return pv.Verify(param);
            });
        }
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Debes crear columnas para poder guardar informaci&oacute;n");
            return;
        }

        // Set SQL Code to action 3
        action3->set_sql_code(
            "INSERT INTO _structbi_space_" + id_space + "._structbi_form_" + form_id->ToString_() + " " \
            "(" + columns + ") VALUES (" + values + ") ");

        // Execute action 3
        self.IdentifyParameters_(action3);
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error fECruxvqCZ: No se pudo guardar el registro. " + action3->get_custom_error());
            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsData::Modify_()
{
    // Function GET /api/forms/data/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/data/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_data_.modify_01_.Setup_(action1);

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    actions_.forms_data_.modify_02_.Setup_(action2);

    // Action 2_1: Get old filepath
    auto action2_1 = function->AddAction_("a2_1");

    // Action 3: Update record
    auto action3 = function->AddAction_("a3");
    actions_.forms_data_.modify_03_.Setup_(action3);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2, action2_1, action3](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": CnMsvrA4aa");
            return;
        }
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": Fr5MHxX1wQ");
            return;
        }

        // Get form ID
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error PPM2dLq5wk");
            return;
        }

        // Get Column ID
        auto column_id = action1->get_results()->front()->ExtractField_("column_id");
        if(column_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error FaAMlFeyJC");
            return;
        }

        // Get columns from action 2
        std::string columns = "";
        for(auto it : *action2->get_results())
        {
            // Get column
            auto id = it.get()->ExtractField_("id");
            auto identifier = it.get()->ExtractField_("identifier");
            auto column_type = it.get()->ExtractField_("column_type");
            auto name = it.get()->ExtractField_("name");
            auto length = it.get()->ExtractField_("length");
            auto required = it.get()->ExtractField_("required");
            auto default_value = it.get()->ExtractField_("default_value");

            // Verify identifier is not null
            if(identifier->IsNull_())
                continue;

            // Verify identifier is not the id
            if(identifier->ToString_() == "id")
                continue;

            // Setup columns and values string
            if(columns == "")
            {
                columns = "_structbi_column_" + id->ToString_() + " = ?";
            }
            else
            {
                columns += ",_structbi_column_" + id->ToString_() + " = ?";
            }

            // Get file manager
            auto file_manager = self.get_file_manager();
            file_manager->set_directory_base(
                NAF::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbi-web-uploaded") + "/" + std::string(id_space)
            );

            // Setup action to get old filepath
            std::string filepath_string = "";
            if(column_type->ToString_() == "image" || column_type->ToString_() == "file")
            {
                action2_1->set_sql_code(
                    "SELECT _structbi_column_" + id->ToString_() + " "
                    "FROM _structbi_space_" + id_space + "._structbi_form_" + form_id->ToString_() + " " \
                    "WHERE _structbi_column_" + column_id->ToString_() + " = ?"
                );
                action2_1->AddParameter_("id", "", true)
                ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
                {
                    if(param->get_value()->ToString_() == "")
                    {
                        param->set_error("El id no puede estar vacío");
                    }

                    return true;
                });
                self.IdentifyParameters_(action2_1);
                if(!action2_1->Work_())
                {
                    self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2_1->get_identifier() + ": yKqkgKKfdg");
                    return;
                }
                auto filepath = action2_1->get_results()->First_();
                if(!filepath->IsNull_())
                    filepath_string = filepath->ToString_();
            }

            // Setup parameter
            action3->AddParameter_(identifier->ToString_(), NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()), true)
            ->SetupCondition_(identifier->ToString_(), Query::ConditionType::kError, [file_manager, length, required, default_value, column_type, filepath_string](Query::Parameter::Ptr param)
            {
                FileProcessing fp;
                fp.file_manager = file_manager;
                fp.param = param;
                fp.modify = true;
                fp.filepath = filepath_string;
                ParameterVerification pv(length, required, default_value, column_type, fp);
                return pv.Verify(param);
            });
        }

        // Add id parameter
        action3->AddParameter_("id", "", true)
        ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
        {
            if(param->get_value()->ToString_() == "")
            {
                param->set_error("El id no puede estar vacío");
            }

            return true;
        });

        // Verify that columns is not empty
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Debes crear columnas para poder guardar informaci&oacute;n");
            return;
        }

        // Set SQL Code to action 3
        action3->set_sql_code(
            "UPDATE _structbi_space_" + id_space + "._structbi_form_" + form_id->ToString_() + " " \
            "SET " + columns + " WHERE _structbi_column_" + column_id->ToString_() + " = ?");

        // Execute action 2
        self.IdentifyParameters_(action3);
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error UyUKjUef7b: No se pudo guardar el registro.");
            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::FormsData::Delete_()
{
    // Function GET /api/forms/data/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/data/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    actions_.forms_data_.delete_a01_.Setup_(action1);

    // Action 2: Save new record
    auto action2 = function->AddAction_("a2");
    actions_.forms_data_.delete_a02_.Setup_(action2);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2](NAF::Functions::Function& self)
    {
        // Execute action 1
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": twQ1cxcgZs");
            return;
        }

        // Get form ID
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error PPM2dLq5wk");
            return;
        }

        // Get Column ID
        auto column_id = action1->get_results()->front()->ExtractField_("column_id");
        if(column_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error LbunnRyAm2");
            return;
        }

        // Action 2: Save new record
        action2->set_sql_code(
            "DELETE FROM _structbi_space_" + id_space + "._structbi_form_" + form_id->ToString_() + 
            " WHERE _structbi_column_" + column_id->ToString_() + " = ?"
        );

        // Execute action 2
        self.IdentifyParameters_(action2);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error VF1ACrujc7");
            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

bool StructBI::Functions::FormsData::ParameterVerification::Verify(Query::Parameter::Ptr param)
{
    if(param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kEmpty))
    {
        // If value is empty
        if(required->Int_() == 1)
        {
            // If value is required
            if(default_value->ToString_() == "")
            {
                // default value is empty
                param->set_error("Este parámetro es obligatorio");
                return false;
            }
            else
                param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(default_value->ToString_())));
        }
        else
        {
            // value is not required
            if(default_value->ToString_() == "")
                return true;
            else
                param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(default_value->ToString_())));
        }
    }
    else if (param->get_value()->TypeIsIqual_(NAF::Tools::DValue::Type::kString))
    {
        // value is a string
        if(param->get_value()->ToString_() == "")
        {
            // if value is empty
            if(default_value->ToString_() == "")
            {
                // if default value is empty
                if(required->Int_() == 1)
                {
                    // if value is required
                    param->set_error("Este parámetro es obligatorio");
                    return false;
                }
                else
                    param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue()));
            }
            else
                param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(default_value->ToString_())));
        }
        else
        {
            // if value is not an empty string
            if(column_type->ToString_() == "file" || column_type->ToString_() == "image")
            {
                // if column type is file or image
                if(file_processing.modify)
                {
                    file_processing.Delete();
                    if(!file_processing.Save())
                        return false;
                }
                else
                {
                    if(!file_processing.Save())
                        return false;
                }

                param->set_value(NAF::Tools::DValue::Ptr(new NAF::Tools::DValue(file_processing.filepath)));
            }
        }
    }

    return true;
}

bool StructBI::Functions::FormsData::FileProcessing::Save()
{
    filepath = "";

    // Setup file manager
    file_manager->AddBasicSupportedFiles_();

    // Upload new file
    file_manager->set_operation_type(Files::OperationType::kUpload);
    auto& front_file = file_manager->get_files().front();
    
    if(!file_manager->ChangePathAndFilename_(front_file, file_manager->get_directory_base()))
    {
        param->set_error("Error al subir el archivo.");
        return false;
    }
    if(!file_manager->IsSupported_())
    {
        param->set_error("Archivo no soportado.");
        return false;
    }
    if(!file_manager->VerifyMaxFileSize_())
    {
        param->set_error("El archivo debe ser de menos de 5MB.");
        return false;
    }
    file_manager->UploadFile_();
    
    filepath = front_file.get_requested_path()->getFileName();
    return true; 
}

bool StructBI::Functions::FormsData::FileProcessing::Delete()
{
    // Verify logo exists and remove it
    Files::FileManager file_manager;
    file_manager.set_directory_base(this->file_manager->get_directory_base());
    file_manager.set_operation_type(Files::OperationType::kDelete);
    file_manager.get_files().push_back(file_manager.CreateTempFile_("/" + filepath));

    if(file_manager.CheckFiles_())
    {
        file_manager.RemoveFile_();
    }
    else
        return false;
    
    return true;
}