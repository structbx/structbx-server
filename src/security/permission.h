
#ifndef STRUCTBX_SECURITY_PERMISSION
#define STRUCTBX_SECURITY_PERMISSION


#include <string>
#include <memory>

#include "tools/route.h"
#include "security/user.h"


namespace StructBX
{
    namespace Security
    {
        enum class ActionType;
        class Permission;
    }
}


enum class StructBX::Security::ActionType
{
    kCreate
    ,kRead
    ,kUpdate
    ,kDelete
};

class StructBX::Security::Permission
{
    public:
        Permission();
        Permission(Tools::Route route_, std::shared_ptr<User> user_, ActionType action_type);

        Tools::Route& get_route()
        {
            auto& var = route_;
            return var;
        }
        std::shared_ptr<User>& get_user()
        {
            auto& var = user_;
            return var;
        }
        ActionType get_action_type() const { return action_type_; }

        void set_route(Tools::Route route) { route_ = route; }
        void set_action_type(ActionType& action_type) { action_type_ = action_type; }

    private:
        Tools::Route route_;
        std::shared_ptr<User> user_;
        ActionType action_type_;
};


#endif // STRUCTBX_SECURITY_PERMISSION
