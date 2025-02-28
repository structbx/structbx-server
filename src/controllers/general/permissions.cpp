
#include "functions/general/permissions.h"

using namespace StructBX::Functions::General;

Permissions::Permissions(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,struct_read_(function_data)
    ,struct_read_out_group_(function_data)
    ,struct_add_(function_data)
    ,struct_delete_(function_data)
{
    
}

Permissions::Read::Read(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/general/permissions/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/permissions/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Permissions::Read::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT ng.*, e.title AS title "
        "FROM permissions ng "
        "JOIN endpoints e ON e.endpoint = ng.endpoint "
        "WHERE id_group = ? "
        "ORDER BY e.title ASC"
    );
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

Permissions::ReadOutGroup::ReadOutGroup(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function GET /api/general/permissions/out/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/permissions/out/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action1 = function->AddAction_("a1");
    A1(action1);

    get_functions()->push_back(function);
}

void Permissions::ReadOutGroup::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT e.* "
        "FROM endpoints e "
        "LEFT JOIN permissions ng ON ng.endpoint = e.endpoint AND id_group = ? "
        "WHERE ng.endpoint IS NULL "
        "ORDER BY e.title ASC"
    );
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}

Permissions::Add::Add(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function POST /api/general/permissions/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/permissions/add", HTTP::EnumMethods::kHTTP_POST);
    
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Verify if permission in group don't exists yet
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Add permission
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([action1, action2](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }
        // Execute actions
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
            return;
        }

        NAF::Security::PermissionsManager::LoadPermissions_();

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void Permissions::Add::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT id "
        "FROM permissions "
        "WHERE endpoint = ? AND id_group = ?"
    );
    action->SetupCondition_("condition-permission-exists", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() > 0)
        {
            self.set_custom_error("Este permiso ya está registrado en este grupo");
            return false;
        }

        return true;
    });

    action->AddParameter_("endpoint", "", true)
    ->SetupCondition_("condition-endpoint", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El endpoint no puede estar vacío");
            return false;
        }
        return true;
    });
    action->AddParameter_("id_group", "", true)
    ->SetupCondition_("condition-id_group", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de grupo no puede estar vacío");
            return false;
        }
        return true;
    });
}
void Permissions::Add::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "INSERT INTO permissions (endpoint, action, id_group) "
        "SELECT ?, action, ? "
        "FROM endpoints "
        "WHERE endpoint = ?"
    );
    action->AddParameter_("endpoint", "", true);
    action->AddParameter_("id_group", "", true);
    action->AddParameter_("endpoint", "", true);
}

Permissions::Delete::Delete(Tools::FunctionData& function_data) : Tools::FunctionData(function_data)
{
    // Function PUT /api/general/permissions/delete
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/general/permissions/delete", HTTP::EnumMethods::kHTTP_DEL);
    
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Verify if group don't exists
    auto action1 = function->AddAction_("a1");
    A1(action1);

    // Delete group
    auto action2 = function->AddAction_("a2");
    A2(action2);

    // Setup Custom Process
    auto id_space = get_space_id();
    function->SetupCustomProcess_([action1, action2](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }
        // Execute actions
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
            return;
        }

        NAF::Security::PermissionsManager::LoadPermissions_();

        self.JSONResponse_(HTTP::Status::kHTTP_OK, "OK.");
    });

    get_functions()->push_back(function);
}

void Permissions::Delete::A1(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code(
        "SELECT id "
        "FROM permissions "
        "WHERE id = ?"
    );
    action->SetupCondition_("condition-permission-exists", Query::ConditionType::kError, [](NAF::Functions::Action& self)
    {
        if(self.get_results()->size() < 1)
        {
            self.set_custom_error("El permiso al que intenta borrar no existe");
            return false;
        }

        return true;
    });

    action->AddParameter_("id", "", true)
    ->SetupCondition_("condition-id", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El id de permiso no puede estar vacío");
            return false;
        }
        return true;
    });
}

void Permissions::Delete::A2(NAF::Functions::Action::Ptr action)
{
    action->set_sql_code("DELETE FROM permissions WHERE id = ?");
    action->AddParameter_("id", "", true);

}