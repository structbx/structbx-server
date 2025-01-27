
#ifndef STRUCTBX_FUNCTIONS_FORMS_PERMISSIONS_H
#define STRUCTBX_FUNCTIONS_FORMS_PERMISSIONS_H

#include "tools/base_action.h"
#include "tools/function_data.h"

#include "functions/forms/data.h"
#include "functions/forms/columns.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Forms
        {
            struct PermissionsData;
            class Permissions;
        }
    }
}

using namespace StructBX;
using namespace NAF;

struct StructBX::Functions::Forms::PermissionsData
{
    PermissionsData(Tools::FunctionData& function_data) : 
        read_a01_(function_data)
    {
    }
    class ReadA01 : public Tools::BaseAction
    {
        public:
            using Tools::BaseAction::BaseAction;
            virtual void Setup_(NAF::Functions::Action::Ptr action) override;
    } read_a01_;
};

class StructBX::Functions::Forms::Permissions : public Tools::FunctionData
{
    public:
        Permissions(Tools::FunctionData& function_data);

    protected:
        void Read_();
        void ReadUsersOut_();

    private:
        PermissionsData actions_;
};

#endif //STRUCTBX_FUNCTIONS_FORMS_PERMISSIONS_H