
#ifndef STRUCTBX_FUNCTIONS_GENERAL_GROUPS_H
#define STRUCTBX_FUNCTIONS_GENERAL_GROUPS_H


#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace General
        {
            class Groups;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::General::Groups : public Tools::FunctionData
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

#endif //STRUCTBX_FUNCTIONS_GENERAL_GROUPS_H