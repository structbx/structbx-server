
#include "functions/dashboards.h"

Dashboards::Dashboards()
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
    action->set_sql_code("SELECT * FROM dashboards");

    functions_.push_back(function);
}

void Dashboards::ReadSpecific_()
{
}

void Dashboards::Add_()
{
}

void Dashboards::Modify_()
{
}

void Dashboards::Delete_()
{
}