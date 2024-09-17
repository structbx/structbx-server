
#include "functions/dashboards.h"

Dashboards::Dashboards(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void Dashboards::Read_()
{
    // Function GET /api/dashboards/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/dashboards/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM dashboards ORDER BY position ASC");

    get_functions()->push_back(function);
}

void Dashboards::ReadSpecific_()
{
    // Function GET /api/dashboards/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/dashboards/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM dashboards WHERE id = ?");

    // Parameters and conditions
    action->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id del dashboard no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}

void Dashboards::Add_()
{
    // Function POST /api/dashboards/add
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/dashboards/add", HTTP::EnumMethods::kHTTP_POST);

    // Action 1: Verify that the dashboard name don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT id FROM dashboards WHERE name = ?");
    action1->SetupCondition_("verify-dashboard-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un dashboard con este nombre ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value().TypeIsIqual_(Tools::DValue::Type::kString))
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Add the new dashboard
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("INSERT INTO dashboards (name, state, privacity, added_to_start, position, description, id_cloud_organization) VALUES (?, ?, ?, ?, ?, ?, 1)");

    action2->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 caracteres");
            return false;
        }
        return true;
    });
    action2->AddParameter_("state", Tools::DValue(""), true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El estado no puede estar vac&iacute;o");
            return false;
        }
        return true;
    });
    action2->AddParameter_("privacity", Tools::DValue(""), true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vac&iacute;a");
            return false;
        }
        return true;
    });
    action2->AddParameter_("added_to_start", Tools::DValue(""), true)
    ->SetupCondition_("condition-added_to_start", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() != "0" && param->get_value().ToString_() != "1")
        {
            param->set_error("A&ntilde;adido a inicio debe ser 0 o 1");
            return false;
        }
        return true;
    });
    action2->AddParameter_("position", Tools::DValue(""), true)
    ->SetupCondition_("condition-position", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("La posici&oacute;n no puede estar vac&iacute;a");
            return false;
        }
        return true;
    });
    
    action2->AddParameter_("description", Tools::DValue(""), true);

    get_functions()->push_back(function);
}

void Dashboards::Modify_()
{
    // Function PUT /api/dashboards/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/dashboards/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify that the dashboard name don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT id FROM dashboards WHERE name = ? AND id != ?");
    action1->SetupCondition_("verify-dashboard-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un dashboard con este nombre ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
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

    // Action 2: Modify dashboard
    auto action2 = function->AddAction_("a1");
    action2->set_sql_code("UPDATE dashboards SET name = ?, state = ?, privacity = ?, added_to_start = ?, position = ?, description = ? WHERE id = ?");

    // Parameters and conditions
    action2->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 caracteres");
            return false;
        }
        return true;
    });
    action2->AddParameter_("state", Tools::DValue(""), true)
    ->SetupCondition_("condition-state", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El estado no puede estar vac&iacute;o");
            return false;
        }
        return true;
    });
    action2->AddParameter_("privacity", Tools::DValue(""), true)
    ->SetupCondition_("condition-privacity", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("La privacidad no puede estar vac&iacute;a");
            return false;
        }
        return true;
    });
    action2->AddParameter_("added_to_start", Tools::DValue(""), true)
    ->SetupCondition_("condition-added_to_start", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() != "0" && param->get_value().ToString_() != "1")
        {
            param->set_error("A&ntilde;adido a inicio debe ser 0 o 1");
            return false;
        }
        return true;
    });
    action2->AddParameter_("position", Tools::DValue(""), true)
    ->SetupCondition_("condition-position", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("La posici&oacute;n no puede estar vac&iacute;a");
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

void Dashboards::Delete_()
{
    // Function DEL /api/dashboards/delete
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/dashboards/delete", HTTP::EnumMethods::kHTTP_DEL);

    auto action = function->AddAction_("a1");
    action->set_sql_code("DELETE FROM dashboards WHERE id = ?");

    // Parameters and conditions
    action->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El identificador del dashboard no puede estar vacío");
            return false;
        }
        return true;
    });

    get_functions()->push_back(function);
}