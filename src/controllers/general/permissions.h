
#ifndef STRUCTBX_FUNCTIONS_GENERAL_PERMISSIONS_H
#define STRUCTBX_FUNCTIONS_GENERAL_PERMISSIONS_H


#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace General
        {
            class Permissions;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::General::Permissions : public Tools::FunctionData
{
    public:
        Permissions(Tools::FunctionData& function_data);
        
    protected:
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct ReadOutGroup : public Tools::FunctionData
        {
            ReadOutGroup(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);

            void A1(NAF::Functions::Action::Ptr action);
            void A2(NAF::Functions::Action::Ptr action);
        };

    private:
        Read struct_read_;
        ReadOutGroup struct_read_out_group_;
        Add struct_add_;
        Delete struct_delete_;
};

#endif //STRUCTBX_FUNCTIONS_GENERAL_PERMISSIONS_H