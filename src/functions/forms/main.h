
#ifndef STRUCTBX_FUNCTIONS_FORMS_MAIN_H
#define STRUCTBX_FUNCTIONS_FORMS_MAIN_H

#include "tools/base_action.h"
#include "tools/function_data.h"

#include "functions/forms/data.h"
#include "functions/forms/columns.h"
#include "functions/forms/permissions.h"

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
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
            void A3_1(NAF::Functions::Action::Ptr action);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };

    private:
        Data function_data_;
        Columns function_columns_;
        Permissions function_permissions_;
        Read struct_read_;
        ReadSpecific struct_read_specific_;
        Add struct_add_;
        Modify struct_modify_;
        Delete struct_delete_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_MAIN_H