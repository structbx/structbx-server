
#include "actions/spaces_data.h"

using namespace StructBI;

Actions::SpacesData::SpacesData(FunctionData& function_data) : 
    read_a01_(function_data)
    ,read_specific_a01_(function_data)
    ,change_a01_(function_data)
    ,modify_a01_(function_data)
{
    
}

Actions::SpacesData::ReadA01::ReadA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::SpacesData::ReadA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);

}

Actions::SpacesData::ReadSpecificA01::ReadSpecificA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::SpacesData::ReadSpecificA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    if(get_space_id() == "")
    {
        // Request space ID in DB (Not found in cookie)
        action_->set_sql_code(
            "SELECT s.* " \
            "FROM spaces s " \
            "JOIN spaces_users su ON su.id_space = s.id " \
            "WHERE su.id_naf_user = ?"
        );
        action_->AddParameter_("id_naf_user", get_id_user(), false);
    }
    else
    {
        // Request space ID in Cookie
        action_->set_sql_code(
            "SELECT s.* " \
            "FROM spaces s " \
            "JOIN spaces_users su ON su.id_space = s.id " \
            "WHERE su.id_naf_user = ? AND s.id = ?"
        );
        action_->AddParameter_("id_naf_user", get_id_user(), false);
        action_->AddParameter_("id_space", get_space_id(), false);
    }
}

Actions::SpacesData::ChangeA01::ChangeA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::SpacesData::ChangeA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT s.id, s.name, s.state, s.logo, s.description, s.created_at " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", Tools::DValue::Ptr(new Tools::DValue()), true)
    ->SetupCondition_("condition-id_space", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de cambio de espacio no puede estar vacío");
            return false;
        }
        return true;
    });
}

Actions::SpacesData::ModifyA01::ModifyA01(FunctionData& function_data) : 
    Actions::BaseAction(function_data) 
{

}

void Actions::SpacesData::ModifyA01::Setup_(Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "UPDATE spaces o " \
        "JOIN spaces_users ou ON ou.id_space = o.id " \
        "SET o.name = ?, o.description = ? " \
        "WHERE ou.id_naf_user = ? AND o.id = ?"
    );

    // Parameters and conditions
    action_->AddParameter_("name", "", true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(!param->get_value()->TypeIsIqual_(Tools::DValue::Type::kString))
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
    action_->AddParameter_("description", "", true);

    action_->AddParameter_("id_naf_user", get_id_user(), false);
    action_->AddParameter_("id_space", get_space_id(), false);

}