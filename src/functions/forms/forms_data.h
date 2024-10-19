
#ifndef STRUCTBI_FUNCTIONS_FORMSDATA
#define STRUCTBI_FUNCTIONS_FORMSDATA

#include "tools/function_data.h"

namespace StructBI
{
    namespace Functions
    {
        class FormsData;
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::FormsData : public Tools::FunctionData
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
        void Delete_();
};

#endif //STRUCTBI_FUNCTIONS_FORMSDATA