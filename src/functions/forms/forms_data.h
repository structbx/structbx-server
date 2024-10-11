
#ifndef STRUCTBI_FUNCTIONS_FORMSDATA
#define STRUCTBI_FUNCTIONS_FORMSDATA

#include "function_data.h"

class FormsData : public FunctionData
{
    public:
        FormsData(FunctionData& function_data);

    protected:
        struct ParameterVerification
        {
            bool Verify(Query::Parameter::Ptr param, Query::Field::Ptr length, Query::Field::Ptr required, Query::Field::Ptr default_value);
        };

        void Read_();
        void ReadColumns_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
};

#endif //STRUCTBI_FUNCTIONS_FORMSDATA