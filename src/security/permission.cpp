
#include "security/permission.h"

using namespace StructBX::Security;

Permission::Permission() :
    route_()
    ,user_()
    ,action_type_(ActionType::kRead)
{

}

Permission::Permission(Tools::Route route, std::shared_ptr<User> user, ActionType action_type) :
    route_(route)
    ,user_(user)
    ,action_type_(action_type)
{

}
