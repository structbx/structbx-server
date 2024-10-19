
#ifndef STRUCTBI_FUNCTIONS_FORMS
#define STRUCTBI_FUNCTIONS_FORMS

#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBI
{
    namespace Functions
    {
        class Forms;
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::Forms : public Tools::FunctionData
{
    public:
        Forms(Tools::FunctionData& function_data);

    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;
};

#endif //STRUCTBI_FUNCTIONS_FORMS