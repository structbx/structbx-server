
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
        void Read_();
        void ReadSpecific_();
        void ReadTypes_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_COLUMNS_H