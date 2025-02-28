
#ifndef STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H
#define STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H

#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            class Columns;
        }
    }
}

using namespace StructBX;
using namespace NAF;

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
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct ReadTypes : public Tools::FunctionData
        {
            ReadTypes(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
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
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadSpecific struct_read_specific_;
        ReadTypes struct_read_types_;
        Add struct_add_;
        Modify struct_modify_;
        Delete struct_delete_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H