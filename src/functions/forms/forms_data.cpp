
#include "functions/forms/forms_data.h"

FormsData::FormsData(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    ReadColumns_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void FormsData::Read_()
{
    // Function GET /api/forms/data/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/data/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Get form columns
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action1->set_final(false);
    action1->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id_space", get_space_id(), false);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
    {
        // Get actions
        auto action1 = self.GetAction_("a1");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error QFt5mE0RfV");
            return;
        }

        // Execute action
        if(!action1->get()->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get()->get_identifier() + ": MS46GLPi6D");
            return;
        }

        // Get JSON Results
        auto json_result1 = action1->get()->CreateJSONResult_();

        // Get form info
        auto form_identifier = self.GetParameter_("identifier");

        if(form_identifier == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error HPqWlZkEbk");
            return;
        }

        // Get columns
        std::string columns = "";
        for(auto it : *action1->get()->get_results())
        {
            Query::Field::Ptr identifier = it.get()->ExtractField_("identifier");
            Query::Field::Ptr name = it.get()->ExtractField_("name");
            if(identifier->IsNull_() || name->IsNull_())
                continue;

            if(it == *action1->get()->get_results()->begin())
                columns = identifier->ToString_() + " AS '" + name->ToString_() + "'";
            else
                columns += ", " + identifier->ToString_() + " AS '" + name->ToString_() + "'";
        }

        // Verify if columns is empty
        if(columns == "")
            columns = "*";

        // Action 2: Get Form data
        auto action2 = self.AddAction_("a2");
        action2->set_sql_code(
            "SELECT " + columns + " " \
            "FROM form_" + id_space + "_" + form_identifier->get()->get_value()->ToString_());
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error UgOMMObhM2");
            return;
        }

        // Action 2 results
        auto json_result2 = action2->CreateJSONResult_();
        json_result2->set("status", action2->get_status());
        json_result2->set("message", action2->get_message());
        json_result2->set("columns_meta", json_result1);

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_result2);
    });

    get_functions()->push_back(function);
}

void FormsData::ReadColumns_()
{
    // Function GET /api/forms/data/columns/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/data/columns/read", HTTP::EnumMethods::kHTTP_GET);

    // Action 1: Get current identifier and id_space
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ? AND id_space = ?");
    action1->set_final(false);
    action1->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id_space", get_space_id(), false);

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ? AND fc.identifier != 'id'"
    );
    action2->AddParameter_("identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action2->AddParameter_("id_space", get_space_id(), false);

    get_functions()->push_back(function);
}

void FormsData::ReadSpecific_()
{
    // Function GET /api/forms/data/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/data/read/id", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Get form columns
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action1->set_final(false);
    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id_space", get_space_id(), false);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
    {
        // Get actions
        auto action1 = self.GetAction_("a1");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error QFt5mE0RfV");
            return;
        }

        // Execute action
        if(!action1->get()->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get()->get_identifier() + ": MS46GLPi6D");
            return;
        }

        // Get JSON Results
        auto json_result1 = action1->get()->CreateJSONResult_();

        // Get form info
        auto form_identifier = self.GetParameter_("form-identifier");

        if(form_identifier == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error HPqWlZkEbk");
            return;
        }

        // Get columns
        std::string columns = "";
        for(auto it : *action1->get()->get_results())
        {
            Query::Field::Ptr identifier = it.get()->ExtractField_("identifier");
            Query::Field::Ptr name = it.get()->ExtractField_("name");
            if(identifier->IsNull_() || name->IsNull_())
                continue;

            if(columns == "")
                columns = identifier->ToString_() + " AS '" + name->ToString_() + "'";
            else
                columns += ", " + identifier->ToString_() + " AS '" + name->ToString_() + "'";
        }

        // Verify if columns is empty
        if(columns == "")
            columns = "*";

        // Action 2: Get Form data
        auto action2 = self.AddAction_("a2");
        action2->set_sql_code(
            "SELECT " + columns + " " \
            "FROM form_" + id_space + "_" + form_identifier->get()->get_value()->ToString_() + " " \
            "WHERE id = ?");

        // Add id parameter to action 2
        action2->AddParameter_("id", "", true)
        ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
        {
            if(param->get_value()->ToString_() == "")
            {
                param->set_error("El id no puede estar vacío");
                return false;
            }
            return true;
        });

        // Identify parameters and work
        self.IdentifyParameters_(action2);
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error UgOMMObhM2");
            return;
        }

        // Action 2 results
        auto json_result2 = action2->CreateJSONResult_();
        json_result2->set("status", action2->get_status());
        json_result2->set("message", action2->get_message());
        json_result2->set("columns_meta", json_result1);

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_result2);
    });

    get_functions()->push_back(function);
}

void FormsData::Add_()
{
    // Function GET /api/forms/data/add
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/data/add", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ?");
    action1->set_final(false);
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action2->set_final(false);
    action2->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action2->AddParameter_("id_space", get_space_id(), false);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
    {
        // Verify if exists actions
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error fpnd6GDTxF");
            return;
        }

        // Execute actions
        for(auto action : self.get_actions())
        {
            if(!action->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": nrjlOllSqm");
                return;
            }
        }

        // Get form info
        auto form_identifier = self.GetParameter_("form-identifier");

        if(form_identifier == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error inbZDVtXXd");
            return;
        }

        // Action 3: Save new record
        auto action3 = self.AddAction_("a3");

        // Get columns from action 2
        auto action2 = self.GetAction_("a2");
        std::string columns = "";
        std::string values = "";
        for(auto it : *action2->get()->get_results())
        {
            // Get column
            auto identifier = it.get()->ExtractField_("identifier");
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
                columns = identifier->ToString_();
                values = "?";
            }
            else
            {
                columns += ", " + identifier->ToString_();
                values += ", ?";
            }

            // Setup parameter
            action3->AddParameter_(identifier->ToString_(), Tools::DValue::Ptr(new Tools::DValue()), true)
            ->SetupCondition_(identifier->ToString_(), Query::ConditionType::kError, [length, required, default_value](Query::Parameter::Ptr param)
            {
                ParameterVerification pv;
                return pv.Verify(param, length, required, default_value);
            });
        }
        if(columns == "")
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Debes crear columnas para poder guardar informaci&oacute;n");
            return;
        }

        // Set SQL Code to action 2
        action3->set_sql_code(
            "INSERT INTO  form_" + id_space + "_" + form_identifier->get()->get_value()->ToString_() + " " \
            "(" + columns + ") VALUES (" + values + ") ");

        // Execute action 2
        self.IdentifyParameters_(action3);
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error VF1ACrujc7");
            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void FormsData::Modify_()
{
    // Function GET /api/forms/data/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/data/modify", HTTP::EnumMethods::kHTTP_PUT);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ?");
    action1->set_final(false);
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Get form columns
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code(
        "SELECT fc.*, fct.identifier AS column_type " \
        "FROM forms_columns fc " \
        "JOIN forms_columns_types fct ON fct.id = fc.id_column_type " \
        "JOIN forms f ON f.id = fc.id_form " \
        "WHERE f.identifier = ? AND f.id_space = ?"
    );
    action2->set_final(false);
    action2->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    action2->AddParameter_("id_space", get_space_id(), false);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
    {
        // Verify if exists actions
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error fpnd6GDTxF");
            return;
        }

        // Execute actions
        for(auto action : self.get_actions())
        {
            if(!action->Work_())
            {
                self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": CnMsvrA4aa");
                return;
            }
        }

        // Get form info
        auto form_identifier = self.GetParameter_("form-identifier");

        if(form_identifier == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 1VxAfjpkhN");
            return;
        }

        // Action 3: Save new record
        auto action3 = self.AddAction_("a3");

        // Get columns from action 2
        auto action2 = self.GetAction_("a2");
        std::string columns = "";
        for(auto it : *action2->get()->get_results())
        {
            // Get column
            auto identifier = it.get()->ExtractField_("identifier");
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
                columns = identifier->ToString_() + " = ?";
            }
            else
            {
                columns += ", " + identifier->ToString_() + " = ?";
            }

            // Setup parameters
            action3->AddParameter_(identifier->ToString_(), Tools::DValue::Ptr(new Tools::DValue()), true)
            ->SetupCondition_(identifier->ToString_(), Query::ConditionType::kError, [length, required, default_value](Query::Parameter::Ptr param)
            {
                ParameterVerification pv;
                return pv.Verify(param, length, required, default_value);
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
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Debes crear columnas para poder guardar informaci&oacute;n");
            return;
        }

        // Set SQL Code to action 3
        action3->set_sql_code(
            "UPDATE form_" + id_space + "_" + form_identifier->get()->get_value()->ToString_() + " " \
            "SET " + columns + " WHERE id = ?");

        // Execute action 2
        self.IdentifyParameters_(action3);
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error VF1ACrujc7");
            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

bool FormsData::ParameterVerification::Verify(Query::Parameter::Ptr param, Query::Field::Ptr, Query::Field::Ptr required, Query::Field::Ptr default_value)
{
    if(param->get_value()->TypeIsIqual_(Tools::DValue::Type::kEmpty))
    {
        if(required->Int_() == 1)
        {
            if(default_value->ToString_() == "")
            {
                param->set_error("Este parámetro es obligatorio");
                return false;
            }
            else
                param->set_value(Tools::DValue::Ptr(new Tools::DValue(default_value->ToString_())));
        }
        else
        {
            if(default_value->ToString_() == "")
                return true;
            else
                param->set_value(Tools::DValue::Ptr(new Tools::DValue(default_value->ToString_())));
        }
    }

    return true;
}

void FormsData::Delete_()
{
    // Function GET /api/forms/data/delete
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/data/delete", HTTP::EnumMethods::kHTTP_DEL);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    // Action 1: Verify form existence
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT identifier, id_space FROM forms WHERE identifier = ?");
    action1->set_final(false);
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() != 1)
        {
            self.set_custom_error("El formulario solicitado no existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier-form", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([id_space](Functions::Function& self)
    {
        // Get action 1
        auto action1 = self.GetAction_("a1");
        if(action1 == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error obLuu4LBe9");
            return;
        }

        // Execute action 1
        if(!action1->get()->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get()->get_identifier() + ": twQ1cxcgZs");
            return;
        }

        // Get form info
        auto form_identifier = self.GetParameter_("form-identifier");

        if(form_identifier == self.get_parameters().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error UFwgqfZp59");
            return;
        }

        // Action 2: Save new record
        auto action2 = self.AddAction_("a2");
        action2->set_sql_code(
            "DELETE FROM form_" + id_space + "_" + form_identifier->get()->get_value()->ToString_() + " WHERE id = ?");

        action2->AddParameter_("id", "", true)
        ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
        {
            if(param->get_value()->ToString_() == "")
            {
                param->set_error("El id de registro no puede estar vacío");
                return false;
            }
            return true;
        });

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
