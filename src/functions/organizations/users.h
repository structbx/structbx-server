
#ifndef STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H
#define STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H


#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Organizations
        {
            class Users;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Organizations::Users : public Tools::FunctionData
{
    public:
        Users(Tools::FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadCurrent_();
        void ReadSpecific_();
        void ModifyCurrentUsername_();
        void ModifyCurrentPassword_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;

};

#endif //STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H