
#include "functions/reports_graphs.h"

ReportsGraphs::ReportsGraphs()
{
    Read_();
    ReadSpecific_();
    Add_();
    Modify_();
    Delete_();
}

void ReportsGraphs::Read_()
{
    // Function GET /api/reports/graphs/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/reports/graphs/read", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code("SELECT id, name, created_at FROM reports_graphs");

    functions_.push_back(function);
}

void ReportsGraphs::ReadSpecific_()
{

}

void ReportsGraphs::Add_()
{

}

void ReportsGraphs::Modify_()
{

}

void ReportsGraphs::Delete_()
{
    
}