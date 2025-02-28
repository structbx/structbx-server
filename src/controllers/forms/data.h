
#ifndef STRUCTBX_FUNCTIONS_FORMS_DATA_H
#define STRUCTBX_FUNCTIONS_FORMS_DATA_H

#include <fstream>

#include "tools/base_action.h"
#include "tools/function_data.h"
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
            class Data;
        }
    }
}

using namespace StructBX;
using namespace NAF;

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

        struct VerifyPermissionsRead : public Tools::FunctionData
        {
            VerifyPermissionsRead(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct VerifyPermissionsAdd : public Tools::FunctionData
        {
            VerifyPermissionsAdd(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct VerifyPermissionsModify : public Tools::FunctionData
        {
            VerifyPermissionsModify(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct VerifyPermissionsDelete : public Tools::FunctionData
        {
            VerifyPermissionsDelete(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct ReadChangeInt : public Tools::FunctionData
        {
            ReadChangeInt(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
        };
        struct ReadFile : public Tools::FunctionData
        {
            ReadFile(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
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
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadChangeInt struct_read_change_int_;
        ReadSpecific struct_read_specific_;
        ReadFile struct_read_file_;
        Add struct_add_;
        Modify struct_modify_;
        Delete struct_delete_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_DATA_H