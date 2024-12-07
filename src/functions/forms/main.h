
#ifndef STRUCTBX_FUNCTIONS_FORMS_MAIN_H
#define STRUCTBX_FUNCTIONS_FORMS_MAIN_H

#include "tools/function_data.h"
#include "tools/actions_data.h"

#include "functions/forms/data.h"
#include "functions/forms/columns.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Forms::Main : public Tools::FunctionData
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

#endif //STRUCTBX_FUNCTIONS_FORMS_MAIN_H