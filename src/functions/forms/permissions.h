
#ifndef STRUCTBX_FUNCTIONS_FORMS_PERMISSIONS_H
#define STRUCTBX_FUNCTIONS_FORMS_PERMISSIONS_H

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
            class Permissions;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Forms::Permissions : public Tools::FunctionData
{
    public:
        Permissions(Tools::FunctionData& function_data);

    protected:
        void Read_();

    private:
        Tools::ActionsData actions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_PERMISSIONS_H