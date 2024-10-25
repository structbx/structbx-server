
#include "functions/spaces/spaces.h"

StructBI::Functions::Spaces::Spaces(Tools::FunctionData& function_data) :
    Tools::FunctionData(function_data)
    ,actions_(function_data)
{
    Read_();
    ReadSpecific_();
    Change_();
    Add_();
    Modify_();
    Delete_();
}

void StructBI::Functions::Spaces::Read_()
{
    // Function GET /api/spaces/read
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/read", HTTP::EnumMethods::kHTTP_GET);
    
    auto action = function->AddAction_("a1");
    actions_.spaces_.read_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void StructBI::Functions::Spaces::ReadSpecific_()
{
    // Function GET /api/spaces/read/id
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/read/id", HTTP::EnumMethods::kHTTP_GET);
    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    actions_.spaces_.read_specific_a01_.Setup_(action);

    function->SetupCustomProcess_([](NAF::Functions::Function& self)
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

void StructBI::Functions::Spaces::Change_()
{
    // Function GET /api/spaces/change
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/change", HTTP::EnumMethods::kHTTP_POST);

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    auto action = function->AddAction_("a1");
    actions_.spaces_.change_a01_.Setup_(action);

    function->SetupCustomProcess_([&](NAF::Functions::Function& self)
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
            auto space_id_encoded = Tools::Base64Tool().Encode_(field->ToString_());
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

void StructBI::Functions::Spaces::Add_()
{
    // Function GET /api/spaces/add
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/add", HTTP::EnumMethods::kHTTP_POST);

    // Verify space if exists
    auto action1 = function->AddAction_("a1");
    actions_.spaces_.add_a01_.Setup_(action1);

    // Add space
    auto action2 = function->AddAction_("a2");
    actions_.spaces_.add_a02_.Setup_(action2);

    // Add current user to the new space
    auto action3 = function->AddAction_("a3");
    actions_.spaces_.add_a03_.Setup_(action3);

    get_functions()->push_back(function);
}

void StructBI::Functions::Spaces::Modify_()
{
    // Function PUT /api/spaces/general/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/general/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Modify space
    auto action = function->AddAction_("a1");
    actions_.spaces_.modify_a01_.Setup_(action);

    get_functions()->push_back(function);
}

void StructBI::Functions::Spaces::Delete_()
{
    
}
