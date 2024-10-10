
#include "functions/spaces/spaces.h"

Spaces::Spaces(FunctionData& function_data) :
    FunctionData(function_data)
{
    Read_();
    ReadSpecific_();
    Change_();
    Add_();
    Modify_();
    Delete_();
}

void Spaces::Read_()
{
    // Function GET /api/spaces/read
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT s.* " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);

    get_functions()->push_back(function);
}

void Spaces::ReadSpecific_()
{
    // Function GET /api/spaces/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(Functions::Function::ResponseType::kCustom);

    auto space_id = get_space_id();
    function->SetupCustomProcess_([space_id](Functions::Function& self)
    {
        // Function to request the current Space
        Functions::Action action("a1");

        if(space_id == "")
        {
            // Request space ID in DB (Not found in cookie)
            action.set_sql_code(
                "SELECT s.* " \
                "FROM spaces s " \
                "JOIN spaces_users su ON su.id_space = s.id " \
                "WHERE su.id_naf_user = ?"
            );
            action.AddParameter_("id_naf_user", self.get_current_user().get_id(), false);
        }
        else
        {
            // Request space ID in Cookie
            action.set_sql_code(
                "SELECT s.* " \
                "FROM spaces s " \
                "JOIN spaces_users su ON su.id_space = s.id " \
                "WHERE su.id_naf_user = ? AND s.id = ?"
            );
            action.AddParameter_("id_naf_user", self.get_current_user().get_id(), false);
            action.AddParameter_("id_space", space_id, false);
        }

        // Execute action
        if(!action.Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action.get_custom_error());
            return;
        }
        auto result = action.get_json_result();

        // Set Space ID Cookie to the client
        auto field = action.get_results()->First_();
        if(!field->IsNull_())
        {
            if(space_id == "")
            {
                // If space is empty, set the cookie
                Net::HTTPCookie cookie("structbi-space-id", field->ToString_());
                cookie.setPath("/");

                auto& response = self.get_http_server_response().value();
                response->addCookie(cookie);
            }

            self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);
        }
        else
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "El usuario no est&aacute; en alg&uacute;n espacio.");
    });

    get_functions()->push_back(function);
}

void Spaces::Change_()
{
    // Function GET /api/spaces/change
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/change", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    action->set_sql_code(
        "SELECT s.id, s.name, s.state, s.logo, s.description, s.created_at " \
        "FROM spaces s " \
        "JOIN spaces_users su ON su.id_space = s.id " \
        "WHERE su.id_naf_user = ? AND s.id = ?"
    );
    action->AddParameter_("id_naf_user", get_id_user(), false);
    action->AddParameter_("id_space", Tools::DValue::Ptr(new Tools::DValue()), true)
    ->SetupCondition_("condition-name", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
    {
        if(param->ToString_() == "")
        {
            param->set_error("El identificador de nuevo espacio no puede estar vacío");
            return false;
        }
        return true;
    });

    function->SetupCustomProcess_([&](Functions::Function& self)
    {
        // Search first action
        auto action = *self.get_actions().begin();
        if(self.get_actions().begin() == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "No actions found.");
            return;
        }
        
        // Execute action
        if(!action->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action->get_custom_error());
            return;
        }
        auto result = action->get_json_result();

        // Set Space ID Cookie to the client
        auto field = action->get_results()->First_();
        if(!field->IsNull_())
        {
            // Set Cookie Space ID
            Net::HTTPCookie cookie("structbi-space-id", field->ToString_());
            cookie.setPath("/");

            auto& response = self.get_http_server_response().value();
            response->addCookie(cookie);
            
            // Send results
            self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);
        }
        else
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "El usuario no est&aacute; en alg&uacute;n espacio.");
    });

    get_functions()->push_back(function);
}

void Spaces::Add_()
{

}

void Spaces::Modify_()
{
    // Function PUT /api/spaces/general/modify
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/general/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Modify space
    auto action1 = function->AddAction_("a1");
    action1->set_sql_code(
        "UPDATE spaces o " \
        "JOIN spaces_users ou ON ou.id_space = o.id " \
        "SET o.name = ?, o.description = ? " \
        "WHERE ou.id_naf_user = ?"
    );

    // Parameters and conditions
    action1->AddParameter_("name", "", true)
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
    action1->AddParameter_("description", "", true);

    action1->AddParameter_("id_naf_user", get_id_user(), false);

    get_functions()->push_back(function);
}

void Spaces::Delete_()
{
    
}
