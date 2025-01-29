
#ifndef STRUCTBX_FUNCTIONS_ORGANIZATIONS_GROUPS_H
#define STRUCTBX_FUNCTIONS_ORGANIZATIONS_GROUPS_H


#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Organizations
        {
            class Groups;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Organizations::Groups : public Tools::FunctionData
{
    public:
        Groups(Tools::FunctionData& function_data);
        
    protected:
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
};

#endif //STRUCTBX_FUNCTIONS_ORGANIZATIONS_GROUPS_H