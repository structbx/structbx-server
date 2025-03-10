
#ifndef STRUCTBX_CONTROLLERS_FORMS_PERMISSIONS_H
#define STRUCTBX_CONTROLLERS_FORMS_PERMISSIONS_H

#include "tools/base_action.h"
#include "tools/function_data.h"

#include "controllers/forms/data.h"
#include "controllers/forms/columns.h"

namespace StructBX
{
    namespace Controllers
    {
        namespace Forms
        {
            class Permissions;
        }
    }
}

using namespace StructBX;

class StructBX::Controllers::Forms::Permissions : public Tools::FunctionData
{
    public:
        Permissions(Tools::FunctionData& function_data);

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
        struct ReadUsersOut : public Tools::FunctionData
        {
            ReadUsersOut(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(StructBX::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadSpecific struct_read_specific_;
        ReadUsersOut struct_read_users_out_;
        Add struct_add_;
        Modify struct_modify_;
        Delete struct_delete_;
};

#endif //STRUCTBX_CONTROLLERS_FORMS_PERMISSIONS_H