
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

    auto space_id = get_space_id();
    auto id_user = get_id_user();
    function->SetupCustomProcess_([space_id, id_user, action](NAF::Functions::Function& self)
    {
        // Get identifier
        auto identifier = self.GetParameter_("identifier");
        if(identifier != self.get_parameters().end())
        {
            action->get_parameters().clear();
            action->set_sql_code(
                "SELECT s.* " \
                "FROM spaces s " \
                "JOIN spaces_users su ON su.id_space = s.id " \
                "WHERE su.id_naf_user = ? AND s.identifier = ?"
            );
            action->AddParameter_("id_naf_user", id_user, false);
            action->AddParameter_("identifier", identifier->get()->ToString_(), true)
            ->SetupCondition_("condition-identifier", Query::ConditionType::kError, [](Query::Parameter::Ptr param)
            {
                if(param->ToString_() == "")
                {
                    param->set_error("El identificador de espacio no puede estar vacío");
                    return false;
                }
                return true;
            });
        }

        if(!action->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action->get_identifier() + ": " + action->get_custom_error());
            return;
        }
        
        self.CompoundResponse_(HTTP::Status::kHTTP_OK, action->get_json_result());
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

    function->set_response_type(NAF::Functions::Function::ResponseType::kCustom);

    // Action1: Verify space if exists
    auto action1 = function->AddAction_("a1");
    actions_.spaces_.add_a01_.Setup_(action1);

    // Action2: Add space
    auto action2 = function->AddAction_("a2");
    actions_.spaces_.add_a02_.Setup_(action2);

    // Action3: Add current user to the new space
    auto action3 = function->AddAction_("a3");
    actions_.spaces_.add_a03_.Setup_(action3);

    // Action4: Create database
    auto action4 = function->AddAction_("a4");

    // Setup Custom Process
    function->SetupCustomProcess_([action1, action2, action3, action4](NAF::Functions::Function& self)
    {
        // Execute actions
        if(!action1->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action1->get_identifier() + ": " + action1->get_custom_error());
            return;
        }
        if(!action2->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action2->get_identifier() + ": " + action2->get_custom_error());
            return;
        }
        if(!action3->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error " + action3->get_identifier() + ": " + action3->get_custom_error());
            return;
        }

        // Get space ID
        auto space_id = action2->get_last_insert_id();
        if(space_id == 0)
        {
            self.JSONResponse_(HTTP::Status::kHTTP_BAD_REQUEST, "Error bq0fyWtqeP");
            return;
        }

        // Create database
        action4->set_sql_code("CREATE DATABASE _structbi_space_" + std::to_string(space_id));
        if(!action4->Work_())
        {
            self.JSONResponse_(HTTP::Status::kHTTP_INTERNAL_SERVER_ERROR, "Error " + action4->get_identifier() + ": No se pudo crear la DB de espacio");

            // Delete space from table
            NAF::Functions::Action action5("a5");
            action5.set_sql_code("DELETE FROM spaces WHERE id = ?");
            action5.AddParameter_("id", std::to_string(space_id), false);

            return;
        }

        // Send results
        self.JSONResponse_(HTTP::Status::kHTTP_OK, "Ok.");
    });

    get_functions()->push_back(function);
}

void StructBI::Functions::Spaces::Modify_()
{
    // Function PUT /api/spaces/modify
    NAF::Functions::Function::Ptr function = 
        std::make_shared<NAF::Functions::Function>("/api/spaces/modify", HTTP::EnumMethods::kHTTP_PUT);

    // Action 1: Verify that current user is in the space
    auto action1 = function->AddAction_("a1");
    actions_.spaces_.modify_a01_.Setup_(action1);

    // Action 2: Verify space identifier
    auto action2 = function->AddAction_("a2");
    actions_.spaces_.modify_a02_.Setup_(action2);

    // Action 3: Modify space
    auto action3 = function->AddAction_("a3");
    actions_.spaces_.modify_a03_.Setup_(action3);

    get_functions()->push_back(function);
}

void StructBI::Functions::Spaces::Delete_()
{
    
}
