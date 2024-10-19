
#ifndef STRUCTBI_FUNCTIONS_FORMSCOLUMNS
#define STRUCTBI_FUNCTIONS_FORMSCOLUMNS

#include "tools/function_data.h"

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
        FormsColumns(Tools::FunctionData& function_data);

    protected:
        void Read_();
        void ReadTypes_();
        void Add_();

};

#endif //STRUCTBI_FUNCTIONS_FORMSCOLUMNS