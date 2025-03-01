
#ifndef STRUCTBX_SECURITY_USERSMANAGER
#define STRUCTBX_SECURITY_USERSMANAGER


#include <string>

#include "Poco/Tuple.h"

#include "functions/action.h"
#include "security/user.h"
#include "tools/output_logger.h"
#include "query/database_manager.h"


namespace StructBX
{
    namespace Security
    {
        class UsersManager;
    }
}


class StructBX::Security::UsersManager
{
    public:
        UsersManager();

        User& get_current_user()
        {
            auto& var = current_user_;
            return var;
        }
        Functions::Action::Ptr get_action()
        {
            auto& var = action_;
            return var;
        }
        Query::DatabaseManager::Credentials& get_credentials()
        {
            auto& var = credentials_;
            return var;
        }

        bool AuthenticateUser_();

    private:
        User current_user_;
        Functions::Action::Ptr action_;
        Query::DatabaseManager::Credentials credentials_;
};


#endif // STRUCTBX_SECURITY_USERSMANAGER
