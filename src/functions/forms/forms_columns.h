
#ifndef STRUCTBI_FUNCTIONS_FORMSCOLUMNS
#define STRUCTBI_FUNCTIONS_FORMSCOLUMNS

#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBI
{
    namespace Functions
    {
        class FormsColumns;
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::FormsColumns : public Tools::FunctionData
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

        FormsColumns(Tools::FunctionData& function_data);

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

#endif //STRUCTBI_FUNCTIONS_FORMSCOLUMNS