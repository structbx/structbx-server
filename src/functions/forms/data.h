
#ifndef STRUCTBI_FUNCTIONS_FORMS_DATA_H
#define STRUCTBI_FUNCTIONS_FORMS_DATA_H

#include "tools/function_data.h"
#include "tools/actions_data.h"
#include <functions/action.h>
#include <functions/function.h>
#include <query/field.h>
#include <query/results.h>

namespace StructBI
{
    namespace Functions
    {
        namespace Forms
        {
            class Data;
        }
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::Forms::Data : public Tools::FunctionData
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

        void ReadChangeInt_();
        void Read_();
        void ReadSpecific_();
        void ReadFile_();
        void Add_();
        void Modify_();
        void ModifyChangeInt_();
        void Delete_();

    private:
        Tools::ActionsData actions_;
};

#endif //STRUCTBI_FUNCTIONS_FORMS_DATA_H