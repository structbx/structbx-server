
#ifndef STRUCTBI_FUNCTIONS_FORMSDATA
#define STRUCTBI_FUNCTIONS_FORMSDATA

#include "function_data.h"

class FormsData : public FunctionData
{
    public:
        FormsData(FunctionData& function_data);

    protected:
        void Read_();
        void ReadColumns_();
        void ReadSpecific_();
        void Add_();
};

#endif //STRUCTBI_FUNCTIONS_FORMSDATA