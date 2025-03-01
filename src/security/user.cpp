
#include "security/user.h"

using namespace StructBX::Security;

User::User() :
    id_(-1)
    ,username_("")
    ,id_group_(-1)
{

}

User::User(int id, std::string username, int id_group) :
    id_(id)
    ,username_(username)
    ,id_group_(id_group)
{

}
