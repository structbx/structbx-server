
#ifndef STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H
#define STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H

#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            struct ColumnsData;
            class Columns;
        }
    }
}

using namespace StructBX;
using namespace NAF;

struct StructBX::Functions::Forms::ColumnsData
{
    ColumnsData(Tools::FunctionData& function_data) : 
        read_a01_(function_data)
        ,read_specific_a01_(function_data)
        ,read_types_a01_(function_data)
        ,add_a01_(function_data)
        ,add_a02_(function_data)
        ,add_a03_(function_data)
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
    class ReadTypesA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_types_a01_;
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

class StructBX::Functions::Forms::Columns : public Tools::FunctionData
{
    public:
        struct ColumnVariables
        {
            std::string column_type = "VARCHAR";
            std::string length = "(100)";
            std::string link_to = "";
            std::string required = "";
            std::string default_value = "";
            std::string cascade_key_condition = "ON DELETE SET NULL ON UPDATE CASCADE";
        };

        struct ColumnSetup
        {
            ColumnSetup(){}
            
            bool Setup(NAF::Functions::Function& self, ColumnVariables& variables);
        };

        struct ColumnTypeSetup
        {
            ColumnTypeSetup(){}
            
            bool Setup(std::string column_type_id, std::string& column_type, std::string& length_value);
        };

        Columns(Tools::FunctionData& function_data);

    protected:
        void Read_();
        void ReadSpecific_();
        void ReadTypes_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        ColumnsData actions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H