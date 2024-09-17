
#include "functions/forms.h"

Forms::Forms(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void Forms::Read_()
{
    // Function GET /api/forms/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM forms");

    get_functions()->push_back(function);
}

void Forms::ReadSpecific_()
{
    // Function GET /api/forms/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM forms WHERE id = ?");

    // Parameters and conditions
    action->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id del formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}

void Forms::Add_()
{
    // Function POST /api/forms/add
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/add", HTTP::EnumMethods::kHTTP_POST);

    // Action 1: Verify that the form identifier don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT id FROM forms WHERE identifier = ?");
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Add the new form
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("INSERT INTO forms (identifier, name, state, description, id_cloud_organization) VALUES (?, ?, ?, ?, 1)");

    action2->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        auto string_param = param->get_value().ToString_();
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(string_param == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(string_param.size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        bool result = IDChecker().Check_(string_param);
        if(!result)
        {
            param->set_error("El identificador solo puede tener a-z, A-Z, 0-9, \"-\" y \"_\"");
            return false;
        }
        return true;
    });
    action2->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action2->AddParameter_("state", Tools::DValue(""), true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("description", Tools::DValue(""), true);

    get_functions()->push_back(function);
}

void Forms::Modify_()
{
    // Function PUT /api/forms/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify that the form identifier don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT id FROM forms WHERE identifier = ? AND id != ?");
    action1->SetupCondition_("verify-form-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un formulario con este identificador ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Modify form
    auto action2 = function->AddAction_("a1");
    action2->set_sql_code("UPDATE forms SET identifier = ?, name = ?, state = ?, description = ? WHERE id = ?");

    // Parameters and conditions
    action2->AddParameter_("identifier", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El identificador debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador no puede estar vacío");
            return false;
        }
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El identificador no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action2->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action2->AddParameter_("state", Tools::DValue(""), true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El estado no puede estar vacío");
            return false;
        }
        return true;
    });
    action2->AddParameter_("description", Tools::DValue(""), true);

    action2->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}

void Forms::Delete_()
{
    // Function DEL /api/forms/delete
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/forms/delete", HTTP::EnumMethods::kHTTP_DEL);

    auto action = function->AddAction_("a1");
    action->set_sql_code("DELETE FROM forms WHERE id = ?");

    // Parameters and conditions
    action->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}