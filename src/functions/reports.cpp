
#include "functions/reports.h"

Reports::Reports()
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void Reports::Read_()
{
    // Function GET /api/reports/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/reports/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT " \
            "r.id AS id, r.name AS name, r.state AS state, r.privacity AS privacity, r.description AS description " \
            ",r.created_at AS created_at, rg.id AS rg_id, rg.name AS rg_name, rg.created_at AS rg_created_at " \
        "FROM reports r " \
        "LEFT JOIN reports_graphs rg ON rg.id = r.id_graph"
    );

    functions_.push_back(function);
}

void Reports::ReadSpecific_()
{
    // Function GET /api/reports/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/reports/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT " \
            "r.id AS id, r.name AS name, r.state AS state, r.privacity AS privacity, r.description AS description " \
            ",r.sql_code AS sql_code, r.created_at AS created_at, rg.id AS rg_id, rg.name AS rg_name, rg.created_at AS rg_created_at " \
        "FROM reports r " \
        "LEFT JOIN reports_graphs rg ON rg.id = r.id_graph " \
        "WHERE r.id = ?"
    );

    // Parameters and conditions
    action->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().ToString_() == "")
        {
            param->set_error("El id del reporte no puede estar vacío");
            return false;
        }
        return true;
    });

    functions_.push_back(function);
}

void Reports::Add_()
{
    // Function POST /api/reports/add
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/reports/add", HTTP::EnumMethods::kHTTP_POST);

    // Action 1: Verify that the dashboard name don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT id FROM reports WHERE name = ?");
    action1->SetupCondition_("verify-reports-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Un reporte con este nombre ya existe");
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

    // Action 2: Add the new report
    auto action2 = function->AddAction_("a2");
    action2->set_sql_code("INSERT INTO reports (name, state, privacity, sql_code, description, id_graph) VALUES (?, ?, ?, ?, ?, ?)");

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
    action2->AddParameter_("sql_code", Tools::DValue(""), true)
    ->SetupCondition_("condition-sql_code", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El c&oacute;digo SQL no puede estar vac&iacute;o");
            return false;
        }
        return true;
    });
    action2->AddParameter_("description", Tools::DValue(""), true);

    action2->AddParameter_("id_graph", Tools::DValue(), true);
    
    functions_.push_back(function);
}

void Reports::Modify_()
{

}

void Reports::Delete_()
{
    
}