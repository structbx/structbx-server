
#include "functions/spaces/spaces.h"

Spaces::Spaces(FunctionData& function_data) :
    FunctionData(function_data)
    ,spaces_actions_(function_data)
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
    spaces_actions_.read_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void Spaces::ReadSpecific_()
{
    // Function GET /api/spaces/read/id
    Functions::Function::Ptr function = 
        std::make_shared<Functions::Function>("/api/spaces/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    spaces_actions_.read_specific_a01_.Setup_(action);

    function->SetupCustomProcess_([](Functions::Function& self)
    {
        // Action to request the current Space
        auto action = self.GetAction_("a1");
        if(action == self.get_actions().end())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error 4H0abNY1yW");
            return;
        }

        // Execute action
        if(!action->get()->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, action->get()->get_custom_error());
            return;
        }
        auto result = action->get()->get_json_result();

        // Set Space ID Cookie to the client
        auto field = action->get()->get_results()->First_();
        if(!field->IsNull_())
            self.CompoundResponse_(HTTP::Status::kHTTP_OK, result);
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
    spaces_actions_.change_a01_.Setup_(action);

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
            auto space_id_encoded = Base64Tool().Encode_(field->ToString_());
            Net::HTTPCookie cookie("1f3efd18688d2b844f4fa1e800712c9b5750c031", space_id_encoded);
            cookie.setPath("/");
            cookie.setSameSite(Net::HTTPCookie::SAME_SITE_STRICT);
            cookie.setSecure(true);
            cookie.setHttpOnly();

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
    auto action = function->AddAction_("a1");
    spaces_actions_.modify_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void Spaces::Delete_()
{
    
}
