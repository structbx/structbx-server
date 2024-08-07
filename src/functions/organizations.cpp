
#include "functions/organizations.h"

Organizations::Organizations(std::string& username)
{
    Read_();
    ReadSpecific_(username);
    Add_();
    Modify_();
    Delete_();
}

void Organizations::Read_()
{

}

void Organizations::ReadSpecific_(std::string& username)
{
    // Function GET /api/organization/general/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/general/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT co.id AS id, co.name AS name, co.state AS state, co.logo AS logo, co.description AS description, co.created_at AS created_at " \
        "FROM cloud_organizations co " \
        "JOIN cloud_users cu ON cu.id_cloud_organization = co.id " \
        "WHERE cu.email = ?"
    );

    // Parameters and conditions
    action->AddParameter_("email", Tools::DValue(username), false);

    functions_.push_back(function);
}

void Organizations::Add_()
{

}

void Organizations::Modify_()
{
    // Function PUT /api/organization/general/modify/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/organization/general/modify/id", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify that the organization name don't exists
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code("SELECT id FROM cloud_organizations WHERE name = ? AND id != ?");
    action1->SetupCondition_("verify-orgniaztion-existence", Query::ConditionType::kError, [](Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Una organizaci&oacute;n con este identificador ya existe");
            return false;
        }

        return true;
    });

    action1->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        return true;
    });

    action1->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    // Action 2: Modify organization
    auto action2 = function->AddAction_("a1");
    action2->set_sql_code("UPDATE cloud_organizations SET name = ?, description = ? WHERE id = ?");

    // Parameters and conditions
    action2->AddParameter_("name", Tools::DValue(""), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value().get_type() != Tools::DValue::Type::kString)
        {
            param->set_error("El nombre debe ser una cadena de texto");
            return false;
        }
        if(param->ToString_() == "")
        {
            param->set_error("El nombre no puede estar vacío");
            return false;
        }
        if(param->ToString_().size() < 3)
        {
            param->set_error("El nombre no puede ser menor a 3 dígitos");
            return false;
        }
        return true;
    });
    action2->AddParameter_("description", Tools::DValue(""), true);

    action2->AddParameter_("id", Tools::DValue(""), true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id no puede estar vacío");
            return false;
        }
        return true;
    });

    functions_.push_back(function);
}

void Organizations::Delete_()
{
    
}