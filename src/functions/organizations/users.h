
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
            struct UsersData;
            class Users;
        }
    }
}

using namespace StructBX;
using namespace NAF;


struct StructBX::Functions::Organizations::UsersData
{
    UsersData(Tools::FunctionData& function_data) : 
        modify_a01_(function_data)
        ,modify_a02_(function_data)
        ,modify_password_a01_(function_data)
        ,modify_password_a02_(function_data)
        ,add_a01_(function_data)
        ,add_a02_(function_data)
        ,modify_user_a01_0_(function_data)
        ,modify_user_a01_(function_data)
        ,modify_user_a02_(function_data)
    {
    }
    class ModifyA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_a01_;
    class ModifyA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_a02_;
    class ModifyPasswordA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_password_a01_;
    class ModifyPasswordA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_password_a02_;
    class AddA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } add_a01_;
    class AddA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } add_a02_;
    class ModifyUserA01_0 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_user_a01_0_;
    class ModifyUserA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_user_a01_;
    class ModifyUserA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_user_a02_;
};

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
        UsersData actions_;

};

#endif //STRUCTBX_FUNCTIONS_ORGANIZATIONS_USERS_H