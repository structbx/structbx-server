
#ifndef STRUCTBX_CONTROLLERS_FORMS_VIEWS_H
#define STRUCTBX_CONTROLLERS_FORMS_VIEWS_H

#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Controllers
    {
        namespace Forms
        {
            class Views;
        }
    }
}

using namespace StructBX;

class StructBX::Controllers::Forms::Views : public Tools::FunctionData
{
    public:
        Views(Tools::FunctionData& function_data);

    protected:
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadSpecific struct_read_specific_;
};

#endif //STRUCTBX_CONTROLLERS_FORMS_VIEWS_H