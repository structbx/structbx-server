
#include "functions/forms/permissions.h"

using namespace StructBX::Functions::Forms;

void PermissionsData::ReadA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fp.*, nu.username AS username, f.name AS form_name " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "JOIN _naf_users nu ON nu.id = fp.id_naf_user "
        "WHERE f.identifier = ? AND f.id_space = ?"
    );

    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void PermissionsData::ReadA02::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->set_sql_code(
        "SELECT fp.*, nu.username AS username, f.name AS form_name " \
        "FROM forms f " \
        "JOIN forms_permissions fp ON fp.id_form = f.id " \
        "JOIN _naf_users nu ON nu.id = fp.id_naf_user "
        "WHERE fp.id = ? AND f.identifier = ? AND f.id_space = ?"
    );

    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El id del permiso de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

void PermissionsData::ModifyA01::Setup_(NAF::Functions::Action::Ptr action)
{
    action_ = action;

    action_->AddParameter_("read", "", true)
    ->SetupCondition_("condition-read", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action_->AddParameter_("add", "", true)
    ->SetupCondition_("condition-add", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action_->AddParameter_("modify", "", true)
    ->SetupCondition_("condition-modify", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action_->AddParameter_("delete", "", true)
    ->SetupCondition_("condition-delete", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action_->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id del permiso de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action_->AddParameter_("id_space", get_space_id(), false);
}

Permissions::Permissions(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    ReadSpecific_();
    ReadUsersOut_();
    Add_();
    Modify_();
}

void Permissions::Read_()
{
    // Function GET /api/forms/permissions/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/permissions/read", HTTP::EnumMethods::kHTTP_GET);

    auto action1 = function->AddAction_("a1");
    actions_.read_a01_.Setup_(action1);

    get_functions()->push_back(function);
}

void Permissions::ReadSpecific_()
{
    // Function GET /api/forms/permissions/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/permissions/read/id", HTTP::EnumMethods::kHTTP_GET);

    auto action1 = function->AddAction_("a1");
    actions_.read_a02_.Setup_(action1);

    get_functions()->push_back(function);
}

void Permissions::ReadUsersOut_()
{
    // Function GET /api/forms/permissions/users/out/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/permissions/users/out/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "SELECT nu.id, nu.username "
        "FROM _naf_users nu "
        "LEFT JOIN forms_permissions su ON "
            "su.id_naf_user = nu.id AND "
            "su.id_form = (SELECT id FROM forms WHERE identifier = ? AND id_space = ?) "
        "WHERE "
            "su.id_naf_user IS NULL "
    );

    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->get_value()->ToString_() == "")
        {
            param->set_error("El identificador del formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action1->AddParameter_("space_id", get_space_id(), false);

    get_functions()->push_back(function);
}

void Permissions::Add_()
{
    // Function GET /api/forms/permissions/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/permissions/add", HTTP::EnumMethods::kHTTP_POST);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "INSERT INTO forms_permissions (`read`, `add`, `modify`, `delete`, id_naf_user, id_form) "
        "SELECT "
            "?, ?, ?, ? "
            ",(SELECT id_naf_user FROM spaces_users WHERE id_naf_user = ? AND id_space = ?) "
            ",(SELECT id FROM forms WHERE identifier = ? AND id_space = ?) "
    );
    action1->AddParameter_("read", "", true)
    ->SetupCondition_("condition-read", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action1->AddParameter_("add", "", true)
    ->SetupCondition_("condition-add", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action1->AddParameter_("modify", "", true)
    ->SetupCondition_("condition-modify", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action1->AddParameter_("delete", "", true)
    ->SetupCondition_("condition-delete", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() != "0" && param->ToString_() != "1")
        {
            param->set_value(std::make_shared<NAF::Tools::DValue>(1));
        }
        return true;
    });
    action1->AddParameter_("id_user", "", true)
    ->SetupCondition_("condition-id_user", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de usuario no puede estar vacío");
            return false;
        }
        return true;
    });
    action1->AddParameter_("id_space", get_space_id(), false);
    action1->AddParameter_("form-identifier", "", true)
    ->SetupCondition_("condition-form-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de formulario no puede estar vacío");
            return false;
        }
        return true;
    });
    action1->AddParameter_("id_space", get_space_id(), false);

    get_functions()->push_back(function);
}

void Permissions::Modify_()
{
    // Function GET /api/forms/permissions/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/permissions/modify", HTTP::EnumMethods::kHTTP_PUT);
    
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "UPDATE forms_permissions "
        "SET `read` = ?, `add` = ?, `modify` = ?, `delete` = ? "
        "WHERE "
            "id = ? "
            "AND id_form = (SELECT id FROM forms WHERE identifier = ? AND id_space = ?) "
    );
    actions_.modify_a01_.Setup_(action1);
    get_functions()->push_back(function);
}
