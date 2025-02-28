
#include "functions/forms/data.h"

using namespace StructBX::Functions;
using namespace StructBX::Functions::Forms;

Forms::Data::Data(Tools::FunctionData& function_data) :
    FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_change_int_(function_data)
    ,struct_read_specific_(function_data)
    ,struct_read_file_(function_data)
    ,struct_add_(function_data)
    ,struct_modify_(function_data)
    ,struct_delete_(function_data)
{
    
}

Forms::Data::VerifyPermissionsRead::VerifyPermissionsRead(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{

}

void Forms::Data::VerifyPermissionsRead::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.read = 1 AND fp.id_naf_user = ?"
    );
    action->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](StructBX::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
    action->AddParameter_("id_user", get_id_user(), false);
}

Forms::Data::VerifyPermissionsAdd::VerifyPermissionsAdd(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    
}

void Forms::Data::VerifyPermissionsAdd::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.add = 1 AND fp.id_naf_user = ?"
    );
    action->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](StructBX::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
    action->AddParameter_("id_user", get_id_user(), false);
}

Forms::Data::VerifyPermissionsModify::VerifyPermissionsModify(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    
}

void Forms::Data::VerifyPermissionsModify::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.modify = 1 AND fp.id_naf_user = ?"
    );
    action->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](StructBX::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
    action->AddParameter_("id_user", get_id_user(), false);
}

Forms::Data::VerifyPermissionsDelete::VerifyPermissionsDelete(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    
}

void Forms::Data::VerifyPermissionsDelete::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fp.delete = 1 AND fp.id_naf_user = ?"
    );
    action->SetupCondition_("verify-permissions", Query::ConditionType::kError, [](StructBX::Functions::Action& action)
    {
        if(action.get_results()->size() < 1)
        {
            action.set_custom_error("No posee los permisos");
            return false;
        }

        return true;
    });
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
    action->AddParameter_("id_user", get_id_user(), false);
}

Forms::Data::Read::Read(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/read
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action 1_0: Get form id
    auto action1_0 = function->AddAction_("a1_0");
    A1(action1_0);

    // Action 1: Get form columns
    auto action1 = function->AddAction_("a1");
    A2(action1);

    // Form permissions verifications
    auto fpv = function->AddAction_("fpv");
    VerifyPermissionsRead struct_verify_permissions_read(function_data);
    struct_verify_permissions_read.A1(fpv);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1_0, action1, fpv](StructBX::Functions::Function& self)
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
        if(!fpv->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Error " + fpv->get_identifier() + ": " + fpv->get_custom_error());
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

            std::string column = "_structbx_column_" + id->ToString_() + " AS '" + name->ToString_() + "'";

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
                column = "_" + link_to->ToString_() + "._structbx_column_" + column2_visualization->ToString_() + " AS '" + name->ToString_() + "'";

                // Setup new join
                joins += " LEFT JOIN _structbx_space_" + id_space + "._structbx_form_" + link_to->ToString_() +
                " AS _" + link_to->ToString_() + " ON _" + link_to->ToString_() + "._structbx_column_" + column2_id->ToString_() + 
                " = _" + form_id->ToString_() + "._structbx_column_" + id->ToString_();
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

        // Get conditions
        auto conditions = self.GetParameter_("conditions");
        std::string condition_query = "";
        if(conditions != self.get_parameters().end())
        {
            if(conditions->get()->ToString_() != "")
            {
                std::string conditions_decoded =  StructBX::Tools::Base64Tool().Decode_(conditions->get()->ToString_());
                condition_query = " WHERE " + conditions_decoded;
            }
        }

        // Get order
        auto order = self.GetParameter_("order");
        std::string order_query = "";
        if(order != self.get_parameters().end())
        {
            if(order->get()->ToString_() != "")
            {
                std::string order_decoded =  StructBX::Tools::Base64Tool().Decode_(order->get()->ToString_());
                order_query = " ORDER BY " + order_decoded;
            }
        }

        // Get page or limit
        auto page = self.GetParameter_("page");
        auto limit = self.GetParameter_("limit");
        int offset = 0;
        std::string limit_query = " LIMIT ";
        if(page != self.get_parameters().end())
        {
            try
            {
                // LIMIT M, N
                offset = (std::stoi(page->get()->ToString_()) - 1) * 20;
                limit_query += std::to_string(offset) + ", 20";
            }
            catch(std::exception&){StructBX::Tools::OutputLogger::Error_("Page parameter is not an integer");}
        }
        else if(limit != self.get_parameters().end())
        {
            try
            {
                // LIMIT N
                limit_query += limit->get()->ToString_();
            }
            catch(std::exception&){StructBX::Tools::OutputLogger::Error_("Limit parameter error");}
        }
        else
        {
            limit_query = "";
        }

        // Export param
        auto export_param = self.GetParameter_("export");

        // Action 2: Get Form data
        auto action2 = self.AddAction_("a2");
        std::string sql_code = 
            "SELECT " + columns + " " \
            "FROM _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + 
                " AS _" + form_id->ToString_()
        ;

        // Prepare JOIN if there is a link
        if(has_link)
            sql_code += joins;

        // Conditions
        sql_code += condition_query;

        // Order
        sql_code += order_query;

        // Limit
        if(export_param == self.get_parameters().end())
        {
            sql_code += limit_query;
        }

        // Execute
        action2->set_sql_code(sql_code);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error UgOMMObhM2");
            return;
        }

        // Send results lambda function
        auto send = [action1, action2](StructBX::Functions::Function& self)
        {
            // Results
            auto json_result1 = action1->get_json_result();
            auto json_result2 = action2->get_json_result();
            json_result2->set("status", action2->get_status());
            json_result2->set("message", action2->get_message());
            json_result2->set("columns_meta", json_result1);

            // Send JSON results
            self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_result2);
        };

        // Get export
        if(export_param != self.get_parameters().end() && export_param->get()->ToString_() == "true")
        {
            // Setup file
            auto fm = Files::FileManager();
            fm.set_operation_type(Files::OperationType::kUpload);
            fm.AddSupportedFile_("csv", Files::FileProperties{"text/csv", false, {""}});
            Files::File file_naf("tmp_export", "tmp_export.csv", "", 0);

            // Change filename
            if(!fm.ChangePathAndFilename_(file_naf, "/tmp"))
            {
                send(self);
                return;
            }

            // File setup
            std::ofstream tmp_file;
            tmp_file.open (file_naf.get_requested_file()->path());

            // Write file
            bool first = true;
            for(auto row : *action2->get_results())
            {
                // Save columns
                if(first)
                {
                    for(auto field : *row)
                    {
                        tmp_file << field->get_column_name() + "\t";
                    }
                    tmp_file << "\n";
                    first = false;
                }
                else
                {
                    // Save data
                    for(auto field : *row)
                    {
                        if(field->IsNull_())
                            tmp_file << "\t";
                        else
                            tmp_file << field->ToString_() + "\t";
                    }
                    tmp_file << "\n";
                }

            }
            tmp_file.close();

            // Send JSON results
            auto fm2 = Files::FileManager(Files::OperationType::kDownload);
            fm2.AddSupportedFile_("csv", Files::FileProperties{"text/csv", false, {""}});
            self.FileResponse_(HTTP::Status::kHTTP_OK, file_naf.get_requested_file()->path(), fm2);
        }
        else
        {
            send(self);
        }
    });

    get_functions()->push_back(function);
}

void Forms::Data::Read::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fc.identifier = 'id'"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Forms::Data::Read::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? " \
        "ORDER BY fc.position ASC"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

Forms::Data::ReadChangeInt::ReadChangeInt(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/read/changeInt
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/read/changeInt", HTTP::EnumMethods::kHTTP_GET);

    // Action 1: Get Change int
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Forms::Data::ReadChangeInt::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.change_int "
        "FROM forms f "
        "WHERE f.identifier = ? AND f.id_space = ?"
    );

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_space", get_space_id(), false);
}

Forms::Data::ReadSpecific::ReadSpecific(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/read/id
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/read/id", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action 1_0: Get form id
    auto action1_0 = function->AddAction_("a1_0");
    A1(action1_0);

    // Action 1: Get form columns
    auto action1 = function->AddAction_("a1");
    A2(action1);

    // Action 2: Get Form data
    auto action2 = function->AddAction_("a2");
    A3(action2);

    // Form permissions verifications
    auto fpv = function->AddAction_("fpv");
    VerifyPermissionsRead struct_verify_permissions_read(function_data);
    struct_verify_permissions_read.A1(fpv);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1_0,action1, action2, fpv](StructBX::Functions::Function& self)
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
        if(!fpv->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Error " + fpv->get_identifier() + ": " + fpv->get_custom_error());
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
                columns = "_structbx_column_" + id->ToString_() + " AS '" + name->ToString_() + "'";
            else
                columns += ", _structbx_column_" + id->ToString_() + " AS '" + name->ToString_() + "'";
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
            "FROM _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + " " \
            "WHERE _structbx_column_" + column_id->ToString_() + " = ?");

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

void Forms::Data::ReadSpecific::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fc.identifier = 'id'"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Forms::Data::ReadSpecific::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT " \
            "fc.*, fct.identifier AS column_type, fct.name AS column_type_name, f.id AS form_id " \
            ",(SELECT identifier FROM forms WHERE id = fc.link_to) AS link_to_form " \
            ",(SELECT name FROM forms WHERE id = fc.link_to) AS link_to_form_name " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? " \
        "ORDER BY fc.position ASC"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Forms::Data::ReadSpecific::A3(StructBX::Functions::Action::Ptr action)
{
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

Forms::Data::ReadFile::ReadFile(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/file/read
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/file/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action 1: Get form id
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Form permissions verifications
    auto fpv = function->AddAction_("fpv");
    VerifyPermissionsRead struct_verify_permissions_read(function_data);
    struct_verify_permissions_read.A1(fpv);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, fpv](StructBX::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Archivo no encontrado en el formulario actual");
            return;
        }
        if(!fpv->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Error " + fpv->get_identifier() + ": " + fpv->get_custom_error());
            return;
        }

        // Get filepath
        auto filepath = self.GetParameter_("filepath");
        if(filepath == self.get_parameters().end())
        {
            self.HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Archivo no encontrado en el formulario actual");
            return;
        }

        // Get form_id
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.HTMLResponse_(HTTP::Status::kHTTP_NOT_FOUND, "Archivo no encontrado en el formulario actual");
            return;
        }

        // Setup file manager
        self.get_file_manager()->AddBasicSupportedFiles_();
        self.get_file_manager()->set_directory_base(
            StructBX::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded") + "/" + std::string(id_space) + "/" + form_id->ToString_()
        );

        // Download process
        auto string_path = filepath->get()->ToString_();
        self.DownloadProcess_(string_path);
    });

    get_functions()->push_back(function);
}

void Forms::Data::ReadFile::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id " \
        "FROM forms f " \
        "WHERE f.identifier = ? AND f.id_space = ? "
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

Forms::Data::Add::Add(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/add
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Save new record
    auto action3 = function->AddAction_("a3");

    // Form permissions verifications
    auto fpv = function->AddAction_("fpv");
    VerifyPermissionsAdd(function_data).A1(fpv);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2, action3, fpv](StructBX::Functions::Function& self)
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
        if(!fpv->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Error " + fpv->get_identifier() + ": " + fpv->get_custom_error());
            return;
        }

        // Get form ID
        auto form_id = action1->get_results()->First_();
        if(form_id->IsNull_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error PPM2dLq5wk");
            return;
        }

        // Configure parameters
        std::string columns = "";
        std::string values = "";
        ParameterConfiguration pc(ParameterConfiguration::Type::kAdd, columns, values, id_space);
        pc.Setup(self, action2->get_results(), form_id, nullptr, action3);

        // Verify that columns is not empty
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Debes crear columnas para poder guardar informaci&oacute;n");
            return;
        }

        // Set SQL Code to action 3
        action3->set_sql_code(
            "INSERT INTO _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + " " \
            "(" + columns + ") VALUES (" + values + ") ");

        // Execute action 3
        self.IdentifyParameters_(action3);
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error fECruxvqCZ: No se pudo guardar el registro. " + action3->get_custom_error());
            return;
        }

        // ChangeInt
        auto form_identifier = self.GetParameter_("form-identifier");
        if(form_identifier != self.get_parameters().end())
        {
            auto changeInt = ChangeInt();
            changeInt.Change(form_identifier->get()->ToString_(), id_space);
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void Forms::Data::Add::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id_space = ?");
    action->set_final(false);
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_space", get_space_id(), false);

}

void Forms::Data::Add::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

Forms::Data::Modify::Modify(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/modify
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Action 3: Update record
    auto action3 = function->AddAction_("a3");

    // Form permissions verifications
    auto fpv = function->AddAction_("fpv");
    VerifyPermissionsModify(function_data).A1(fpv);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2, action3, fpv](StructBX::Functions::Function& self)
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
        if(!fpv->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Error " + fpv->get_identifier() + ": " + fpv->get_custom_error());
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

        // Configure parameters
        std::string columns = "";
        std::string values = "";
        ParameterConfiguration pc(ParameterConfiguration::Type::kModify, columns, values, id_space);
        pc.Setup(self, action2->get_results(), form_id, column_id, action3);

        // Verify that columns is not empty
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Debes crear columnas para poder guardar informaci&oacute;n");
            return;
        }

        // Action3: Add id parameter
        action3->AddParameter_("id", "", true)
        ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
        {
            if(param->get_value()->ToString_() == "")
            {
                param->set_error("El id no puede estar vacío");
            }

            return true;
        });

        // Set SQL Code to action 3
        action3->set_sql_code(
            "UPDATE _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + " " \
            "SET " + columns + " WHERE _structbx_column_" + column_id->ToString_() + " = ?");

        // Execute action 3
        self.IdentifyParameters_(action3);
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error UyUKjUef7b: No se pudo guardar el registro.");
            return;
        }

        // ChangeInt
        auto form_identifier = self.GetParameter_("form-identifier");
        if(form_identifier != self.get_parameters().end())
        {
            auto changeInt = ChangeInt();
            changeInt.Change(form_identifier->get()->ToString_(), id_space);
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void Forms::Data::Modify::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND id_space = ? AND fc.identifier = 'id'"
    );
    action->set_final(false);
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Forms::Data::Modify::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

Forms::Data::Delete::Delete(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/forms/data/delete
    StructBX::Functions::Function::Ptr function = 
        std::make_shared<StructBX::Functions::Function>("/api/forms/data/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(StructBX::Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Action 2_0: Get form columns
    auto action2_0 = function->AddAction_("a2_0");
    A2(action2_0);

    // Action 2: Delete record from table
    auto action2 = function->AddAction_("a2");
    A3(action2);

    // Form permissions verifications
    auto fpv = function->AddAction_("fpv");
    VerifyPermissionsDelete(function_data).A1(fpv);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space, action1, action2_0, action2, fpv](StructBX::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": twQ1cxcgZs");
            return;
        }
        if(!action2_0->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2_0->get_identifier() + ": PYaZ1nddvm");
            return;
        }
        if(!fpv->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_UNAUTHORIZED, "Error " + fpv->get_identifier() + ": " + fpv->get_custom_error());
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

        // Delete record files
        for(auto it : *action2_0->get_results())
        {
            // Get column
            auto id = it.get()->ExtractField_("id");
            auto identifier = it.get()->ExtractField_("identifier");
            auto column_type = it.get()->ExtractField_("column_type");

            // Verify identifier is not null
            if(identifier->IsNull_())
                continue;

            // Verify if is image or file
            if(column_type->IsNull_())
                continue;
            bool r = false;
            if(column_type->ToString_() == "image" || column_type->ToString_() == "file")
                r = true;
            if(!r)
                continue;

            // Get file manager
            auto file_manager = self.get_file_manager();
            file_manager->set_directory_base(
                StructBX::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded") + "/" + std::string(id_space) + "/" + form_id->ToString_()
            );

            // Request filepath
            auto action2_2 = self.AddAction_("a2_2");
            action2_2->set_sql_code(
                "SELECT _structbx_column_" + id->ToString_() + " "
                "FROM _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + " " \
                "WHERE _structbx_column_" + column_id->ToString_() + " = ?"
            );
            action2_2->AddParameter_("id", "", true)
            ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
            {
                if(param->get_value()->ToString_() == "")
                {
                    param->set_error("El id no puede estar vacío");
                }

                return true;
            });
            self.IdentifyParameters_(action2_2);
            if(!action2_2->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2_2->get_identifier() + ": PIvGrSKDYx");
                return;
            }
            auto filepath = action2_2->get_results()->First_();

            // Process file
            FileProcessing fp;
            fp.file_manager = file_manager;
            if(!filepath->IsNull_() && filepath->ToString_() != "")
            {
                fp.filepath = filepath->ToString_();
                fp.Delete();
            }
        }

        // Action 2: Delete record from table
        action2->set_sql_code(
            "DELETE FROM _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + 
            " WHERE _structbx_column_" + column_id->ToString_() + " = ?"
        );

        // Execute action 2
        self.IdentifyParameters_(action2);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error VF1ACrujc7");
            return;
        }

        // ChangeInt
        auto form_identifier = self.GetParameter_("form-identifier");
        if(form_identifier != self.get_parameters().end())
        {
            auto changeInt = ChangeInt();
            changeInt.Change(form_identifier->get()->ToString_(), id_space);
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void Forms::Data::Delete::A1(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT f.id, fc.id AS column_id " \
        "FROM forms f " \
        "JOIN forms_columns fc ON fc.id_form = f.id " \
        "WHERE f.identifier = ? AND id_space = ? AND fc.identifier = 'id'"
    );
    action->set_final(false);
    action->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](StructBX::Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Forms::Data::Delete::A2(StructBX::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action->set_final(false);
    action->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action->AddParameter_("id_space", get_space_id(), false);
}

void Forms::Data::Delete::A3(StructBX::Functions::Action::Ptr action)
{
    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id de registro no puede estar vacío");
            return false;
        }
        return true;
    });
}

bool Forms::Data::ParameterVerification::Verify(Query::Parameter::Ptr param)
{
    if(param->get_value()->TypeIsIqual_(StructBX::Tools::DValue::Type::kEmpty))
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
                param->set_value(StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(default_value->ToString_())));
        }
        else
        {
            // value is not required
            if(default_value->ToString_() == "")
                return true;
            else
                param->set_value(StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(default_value->ToString_())));
        }
    }
    else if (param->get_value()->TypeIsIqual_(StructBX::Tools::DValue::Type::kString))
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
                    param->set_value(StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue()));
            }
            else
                param->set_value(StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue(default_value->ToString_())));
        }
    }

    return true;
}

void Forms::Data::ParameterConfiguration::Setup(StructBX::Functions::Function& self, StructBX::Query::Results::Ptr results, StructBX::Query::Field::Ptr form_id, StructBX::Query::Field::Ptr column_id, StructBX::Functions::Action::Ptr action3)
{
    // Setp 1: Iterate over columns
    for(auto it : *results)
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

        // Step 2: Search column type image or file
        if(column_type->ToString_() == "image" || column_type->ToString_() == "file")
        {
            // Get file manager
            auto file_manager = self.get_file_manager();
            auto new_file_manager = std::make_shared<StructBX::Files::FileManager>();
            file_manager->set_directory_base(
                StructBX::Tools::SettingsManager::GetSetting_("directory_for_uploaded_files", "/var/www/structbx-web-uploaded") + "/" + std::string(id_space) + "/" + form_id->ToString_()
            );
            new_file_manager->set_directory_base(file_manager->get_directory_base());

            // Setup current file to new file manager
            auto found = std::find_if(file_manager->get_files().begin(), file_manager->get_files().end(), [identifier](StructBX::Files::File& file)
            {
                return file.get_name() == identifier->ToString_();
            });

            // Step 4: If there is not file, do not touch the column (or filename is empty)
            if(found == file_manager->get_files().end() || found->get_filename() == "")
                continue;
            new_file_manager->get_files().push_back(*found);

            // Process file
            FileProcessing fp;
            fp.file_manager = new_file_manager;

            // Setup columns and values string
            std::string filepath_string = "";
            if(type == Type::kAdd)
            {
                // Setup columns and values string
                if(columns == "")
                {
                    columns = "_structbx_column_" + id->ToString_();
                    values = "?";
                }
                else
                {
                    columns += ",_structbx_column_" + id->ToString_();
                    values += ", ?";
                }
            }
            else if(type == Type::kModify)
            {
                if(columns == "")
                {
                    columns = "_structbx_column_" + id->ToString_() + " = ?";
                }
                else
                {
                    columns += ",_structbx_column_" + id->ToString_() + " = ?";
                }

                // Step 5: Verify old file saved
                auto action2_1 = StructBX::Functions::Action::Ptr(new StructBX::Functions::Action("a2_1"));
                action2_1->set_sql_code(
                    "SELECT _structbx_column_" + id->ToString_() + " "
                    "FROM _structbx_space_" + id_space + "._structbx_form_" + form_id->ToString_() + " " \
                    "WHERE _structbx_column_" + column_id->ToString_() + " = ?"
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
                {
                    if(found == file_manager->get_files().end())
                        continue;
                    else
                        filepath_string = filepath->ToString_();
                }
                
                // Save filepath found
                fp.filepath = filepath_string;

                // Step 6: if there is old file, delete it
                if(fp.filepath != "")
                {
                    if(!fp.Delete())
                    {
                        self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error en par&aacute;metro (" + identifier->ToString_() + "): " + fp.error);
                        return;
                    }
                }
            }

            // Step 7: Save the new file
            if(!fp.Save())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error en par&aacute;metro (" + identifier->ToString_() + "): " + fp.error);
                return;
            }

            // Setup static parameter
            action3->AddParameter_(identifier->ToString_(), fp.filepath, false)
            ->SetupCondition_(identifier->ToString_(), Query::ConditionType::kError, [length, required, default_value, column_type](Query::Parameter::Ptr param)
            {
                ParameterVerification pv(length, required, default_value, column_type);
                return pv.Verify(param);
            });
        }
        else
        {
            // The type is not image or file
            if(type == Type::kAdd)
            {
                // Setup columns and values string
                if(columns == "")
                {
                    columns = "_structbx_column_" + id->ToString_();
                    values = "?";
                }
                else
                {
                    columns += ",_structbx_column_" + id->ToString_();
                    values += ", ?";
                }
            }
            else if(type == Type::kModify)
            {
                // Setup columns and values string
                if(columns == "")
                {
                    columns = "_structbx_column_" + id->ToString_() + " = ?";
                }
                else
                {
                    columns += ",_structbx_column_" + id->ToString_() + " = ?";
                }
            }
            
            // Setup parameter
            action3->AddParameter_(identifier->ToString_(), StructBX::Tools::DValue::Ptr(new StructBX::Tools::DValue()), true)
            ->SetupCondition_(identifier->ToString_(), Query::ConditionType::kError, [length, required, default_value, column_type](Query::Parameter::Ptr param)
            {
                ParameterVerification pv(length, required, default_value, column_type);
                return pv.Verify(param);
            });
        }
    }
}

bool Forms::Data::FileProcessing::Save()
{
    filepath = "";

    // Setup file manager
    file_manager->AddBasicSupportedFiles_();

    // Upload new file
    file_manager->set_operation_type(Files::OperationType::kUpload);
    auto& front_file = file_manager->get_files().front();
    
    if(!file_manager->ChangePathAndFilename_(front_file, file_manager->get_directory_base()))
    {
        error = "Error al subir el archivo.";
        return false;
    }
    if(!file_manager->IsSupported_())
    {
        error = "Archivo no soportado.";
        return false;
    }
    if(!file_manager->VerifyMaxFileSize_())
    {
        error = "El archivo debe ser de menos de 5MB.";
        return false;
    }
    file_manager->UploadFile_();
    
    filepath = front_file.get_requested_path()->getFileName();
    return true; 
}

bool Forms::Data::FileProcessing::Delete()
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

void Forms::Data::ChangeInt::Change(std::string form_identifier, std::string space_id)
{
    // Action 1: Get Change int
    auto action1 = StructBX::Functions::Action("a1");
    action1.set_sql_code(
        "UPDATE forms "
        "SET change_int = change_int + 1 "
        "WHERE identifier = ? AND id_space = ?"
    );

    action1.AddParameter_("form-identifier", form_identifier, false);
    action1.AddParameter_("id_space", space_id, false);

    // Execute action
    action1.Work_();
}
