
#ifndef STRUCTBX_SECURITY_SECURITYVERIFICATION
#define STRUCTBX_SECURITY_SECURITYVERIFICATION


#include <string>
#include <list>
#include <stdexcept>

#include <Poco/Tuple.h>

#include "security/users_manager.h"
#include "security/permissions_manager.h"
#include "tools/output_logger.h"


namespace StructBX
{
    namespace Security
    {
        enum class SecurityType;
        class SecurityVerification;
    }
}

using namespace Poco;


enum class StructBX::Security::SecurityType
{
    kDisableAll
    ,kEnableAll
};

class StructBX::Security::SecurityVerification
{
    public:
        SecurityVerification();

        Security::PermissionsManager& get_permissions_manager()
        {
            auto& var = permissions_manager_;
            return var;
        }
        Security::UsersManager& get_users_manager()
        {
            auto& var = users_manager_;
            return var;
        }
        Security::SecurityType get_security_type() const { return security_type_; }
        
        void set_security_type(Security::SecurityType security_type) { security_type_ = security_type; }

        bool VerifyRoutesPermissions_(Tools::Route& route, std::string action_type);

    private:
        Security::PermissionsManager permissions_manager_;
        Security::UsersManager users_manager_;
        Security::SecurityType security_type_;
};


#endif // STRUCTBX_SECURITY_SECURITYVERIFICATION
