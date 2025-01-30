
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
        struct ReadSpecific : public Tools::FunctionData
        {
            ReadSpecific(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
            void A3(NAF::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadSpecific struct_read_specific_;
        Add struct_add_;
        Modify struct_modify_;
};

#endif //STRUCTBX_FUNCTIONS_GENERAL_GROUPS_H