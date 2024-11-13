
#ifndef STRUCTBI_FUNCTIONS_FORMS_MAIN_H
#define STRUCTBI_FUNCTIONS_FORMS_MAIN_H

#include "tools/function_data.h"
#include "tools/actions_data.h"

#include "functions/forms/data.h"
#include "functions/forms/columns.h"

namespace StructBI
{
    namespace Functions
    {
        namespace Forms
        {
            class Main;
        }
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::Forms::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);

    protected:
        void Read_();
        void ReadSpecific_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;
        Data data_;
        Columns columns_;
};

#endif //STRUCTBI_FUNCTIONS_FORMS_MAIN_H