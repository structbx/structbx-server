
#ifndef STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H
#define STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H


#include "tools/base_action.h"
#include "tools/function_data.h"

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
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);
        };
        struct ReadCurrent : public Tools::FunctionData
        {
            ReadCurrent(Tools::FunctionData& function_data);
        };
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);
        };
        struct ModifyCurrentUsername : public Tools::FunctionData
        {
            ModifyCurrentUsername(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct ModifyCurrentPassword : public Tools::FunctionData
        {
            ModifyCurrentPassword(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadCurrent struct_read_current_;
        ReadSpecific struct_read_specific_;
        ModifyCurrentUsername struct_modify_current_username_;
        ModifyCurrentPassword struct_modify_current_password_;
        Add struct_add_;
        Modify struct_modify_;
        Delete struct_delete_;

};

#endif //STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H