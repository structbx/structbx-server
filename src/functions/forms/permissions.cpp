
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

Permissions::Permissions(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
}

void Permissions::Read_()
{
    // Function GET /api/forms/permissions/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/forms/permissions/read", HTTP::EnumMethods::kHTTP_GET);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action1 = function->AddAction_("a1");
    actions_.read_a01_.Setup_(action1);

    // Setup custom process
    function->SetupCustomProcess_([action1](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }

        // JSON Results
        auto json_results = action1->CreateJSONResult_();

        // Send results
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, json_results);
    });

    get_functions()->push_back(function);
}
