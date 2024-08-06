
#include "functions/reports_parameters.h"

ReportsParameters::ReportsParameters()
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void ReportsParameters::Read_()
{
    // Function GET /api/reports/parameters/read/
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/reports/parameters/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT * FROM reports_parameters WHERE id_report = ?");

    // Parameters and conditions
    action->AddParameter_("report_id", Tools::DValue(""), true)
    ->SetupCondition_("condition-report_id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
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

void ReportsParameters::ReadSpecific_()
{
    
}

void ReportsParameters::Add_()
{
    
}

void ReportsParameters::Modify_()
{

}

void ReportsParameters::Delete_()
{
    
}