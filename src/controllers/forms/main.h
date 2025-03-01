
#ifndef STRUCTBX_CONTROLLERS_FORMS_MAIN_H
#define STRUCTBX_CONTROLLERS_FORMS_MAIN_H

#include "tools/base_action.h"
#include "tools/function_data.h"

#include "controllers/forms/data.h"
#include "controllers/forms/columns.h"
#include "controllers/forms/permissions.h"

namespace StructBX
{
    namespace Controllers
    {
        namespace Forms
        {
            class Main;
        }
    }
}

using namespace StructBX;


class StructBX::Controllers::Forms::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);

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
            void A2(StructBX::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
            void A2(StructBX::Functions::Action::Ptr action);
            void A3(StructBX::Functions::Action::Ptr action);
            void A3_1(StructBX::Functions::Action::Ptr action);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
            void A2(StructBX::Functions::Action::Ptr action);
            void A3(StructBX::Functions::Action::Ptr action);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
            void A2(StructBX::Functions::Action::Ptr action);
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

#endif //STRUCTBX_CONTROLLERS_FORMS_MAIN_H