
#ifndef STRUCTBX_FUNCTIONS_FORMS_MAIN_H
#define STRUCTBX_FUNCTIONS_FORMS_MAIN_H

#include "tools/function_data.h"
#include "tools/actions_data.h"

#include "functions/forms/data.h"
#include "functions/forms/columns.h"
#include "functions/forms/permissions.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            struct MainData;
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;


struct StructBX::Functions::Forms::MainData
{
    MainData(Tools::FunctionData& function_data) : 
        read_a01_(function_data)
        ,read_specific_a01_(function_data)
        ,read_specific_a02_(function_data)
        ,add_a01_(function_data)
        ,add_a02_(function_data)
        ,add_a03_(function_data)
        ,add_a03_1_(function_data)
        ,modify_a01_(function_data)
        ,modify_a02_(function_data)
        ,modify_a03_(function_data)
        ,delete_a01_(function_data)
        ,delete_a02_(function_data)
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
    class ReadSpecificA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_specific_a02_;
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
    class AddA03_1 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } add_a03_1_;
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
};

class StructBX::Functions::Forms::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);

    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        MainData actions_;
        Data data_;
        Columns columns_;
        Permissions permissions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_MAIN_H