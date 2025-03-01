
#include "security/users_manager.h"

using namespace StructBX::Security;

UsersManager::UsersManager()
{
    // Setting up the action
    action_ = std::make_shared<Functions::Action>("UsersManager::AuthenticateUser_");
    action_->set_sql_code("SELECT id, username, id_group FROM users WHERE username = ? AND password = ?");
    action_->AddParameter_("username", "", true);
    action_->AddParameter_("password", "", true);
}

bool UsersManager::AuthenticateUser_()
{
    try
    {
        // Query process
            action_->Work_();

        // Verify results
            if(action_->get_results()->size() > 0)
            {
                // Set current user
                auto row = action_->get_results()->front();
                auto id = row->ExtractField_("id");
                auto username = row->ExtractField_("username");
                auto id_group = row->ExtractField_("id_group");

                if(id->IsNull_() || username->IsNull_() || id_group->IsNull_())
                {
                    throw std::runtime_error("Error to get results, Null object.");
                    return false;
                }

                current_user_.set_id(id->Int_());
                current_user_.set_username(username->String_());
                current_user_.set_id_group(id_group->Int_());

                return true;
            }
            else
                return false;
    }
    catch(const std::exception& error)
    {
        Tools::OutputLogger::Error_("Error on users_manager.cpp on AuthenticateUser_(): " + std::string(error.what()));
        return false;
    }
}
