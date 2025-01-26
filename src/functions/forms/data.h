
#ifndef STRUCTBX_FUNCTIONS_FORMS_DATA_H
#define STRUCTBX_FUNCTIONS_FORMS_DATA_H

#include <fstream>

#include "tools/function_data.h"
#include "tools/actions_data.h"
#include <functions/action.h>
#include <functions/function.h>
#include <query/field.h>
#include <query/results.h>

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            struct DataData;
            class Data;
        }
    }
}

using namespace StructBX;
using namespace NAF;

struct StructBX::Functions::Forms::DataData
{
    DataData(Tools::FunctionData& function_data) : 
        verify_permissions_read_(function_data)
        ,verify_permissions_add_(function_data)
        ,verify_permissions_modify_(function_data)
        ,verify_permissions_delete_(function_data)
        ,read_a01_0_(function_data)
        ,read_a01_(function_data)
        ,read_file_a01_(function_data)
        ,read_specific_a01_(function_data)
        ,read_specific_a02_(function_data)
        ,add_01_(function_data)
        ,add_02_(function_data)
        ,add_03_(function_data)
        ,modify_01_(function_data)
        ,modify_02_(function_data)
        ,modify_03_(function_data)
        ,delete_a01_(function_data)
        ,delete_a02_(function_data)
    {
    }
    class VerifyPermissionsRead : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } verify_permissions_read_;
    class VerifyPermissionsAdd : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } verify_permissions_add_;
    class VerifyPermissionsModify : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } verify_permissions_modify_;
    class VerifyPermissionsDelete : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } verify_permissions_delete_;
    class ReadA01_0 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_a01_0_;
    class ReadA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_a01_;
    class ReadFileA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_file_a01_;
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
    } add_01_;
    class AddA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } add_02_;
    class AddA03 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } add_03_;
    class ModifyA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_01_;
    class ModifyA02 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_02_;
    class ModifyA03 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } modify_03_;
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

class StructBX::Functions::Forms::Data : public Tools::FunctionData
{
    public:
        Data(FunctionData& function_data);

    protected:
        struct ParameterConfiguration
        {
            enum class Type {kAdd, kModify};

            ParameterConfiguration(Type type, std::string& columns, std::string& values, std::string id_space) :
                type(type)
                ,columns(columns)
                ,values(values)
                ,id_space(id_space)
            {}
            void Setup(NAF::Functions::Function& self, NAF::Query::Results::Ptr results, NAF::Query::Field::Ptr form_id, NAF::Query::Field::Ptr column_id, NAF::Functions::Action::Ptr action3);

            Type type;
            std::string& columns;
            std::string& values;
            std::string id_space;
        };
        struct FileProcessing
        {
            bool Save();
            bool Delete();

            NAF::Files::FileManager::Ptr file_manager;
            std::string filepath = "";
            std::string name = "";
            std::string error = "";
        };
        struct ParameterVerification
        {
            ParameterVerification(
                Query::Field::Ptr length
                ,Query::Field::Ptr required
                ,Query::Field::Ptr default_value
                ,Query::Field::Ptr column_type
            ) :
                length(length)
                ,required(required)
                ,default_value(default_value)
                ,column_type(column_type)
            {
                
            }
            bool Verify(Query::Parameter::Ptr param);

            Query::Field::Ptr length;
            Query::Field::Ptr required;
            Query::Field::Ptr default_value;
            Query::Field::Ptr column_type;
        };
        struct ChangeInt
        {
            void Change(std::string form_identifier, std::string space_id);
        };

        void ReadChangeInt_();
        void Read_();
        void ReadSpecific_();
        void ReadFile_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        DataData actions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_DATA_H