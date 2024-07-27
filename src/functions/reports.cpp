
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

}

void Reports::Add_()
{

}

void Reports::Modify_()
{

}

void Reports::Delete_()
{
    
}