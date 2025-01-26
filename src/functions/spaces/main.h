
#ifndef STRUCTBX_FUNCTIONS_SPACES_MAIN_H
#define STRUCTBX_FUNCTIONS_SPACES_MAIN_H

#include "Poco/DirectoryIterator.h"

#include "tools/function_data.h"
#include "tools/base64_tool.h"

#include "tools/base_action.h"
#include "functions/spaces/users.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Spaces
        {
            struct MainData;
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;


struct StructBX::Functions::Spaces::MainData
{
    MainData(Tools::FunctionData& function_data) : 
        read_a01_(function_data)
        ,read_specific_a01_(function_data)
        ,add_a01_(function_data)
        ,add_a02_(function_data)
        ,add_a03_(function_data)
        ,change_a01_(function_data)
        ,modify_a01_(function_data)
        ,modify_a02_(function_data)
        ,modify_a03_(function_data)
        ,delete_a01_(function_data)
        ,delete_a02_(function_data)
        ,delete_a03_(function_data)
    {
    }
    class ReadA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_a01_;
    class ReadSpecificA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_specific_a01_;
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
    class AddA03 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } add_a03_;
    class ChangeA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } change_a01_;
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
    class ModifyA03 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_a03_;
    class DeleteA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } delete_a01_;
    class DeleteA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } delete_a02_;
    class DeleteA03 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } delete_a03_;
};

class StructBX::Functions::Spaces::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadSpecific_();
        void Change_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        MainData actions_;
        Users users_;
};

#endif //STRUCTBX_FUNCTIONS_SPACES_MAIN_H