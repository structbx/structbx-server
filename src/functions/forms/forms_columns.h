
#ifndef STRUCTBI_FUNCTIONS_FORMSCOLUMNS
#define STRUCTBI_FUNCTIONS_FORMSCOLUMNS

#include "function_data.h"

class FormsColumns : public FunctionData
{
    public:
        FormsColumns(FunctionData& function_data);

    protected:
        void Read_();
        void ReadTypes_();
        void Add_();

};

#endif //STRUCTBI_FUNCTIONS_FORMSCOLUMNS