
#ifndef STRUCTBX_FUNCTIONS_GENERAL_MAIN_H
#define STRUCTBX_FUNCTIONS_GENERAL_MAIN_H


#include "tools/base_action.h"
#include "tools/function_data.h"

#include "functions/general/users.h"
#include "functions/general/groups.h"

namespace StructBX
{
    namespace Functions
    {
        namespace General
        {
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::General::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);
        
    protected:
        struct ReadInstanceName : public Tools::FunctionData
        {
            ReadInstanceName(Tools::FunctionData& function_data);
        };
        struct ModifyInstanceName : public Tools::FunctionData
        {
            ModifyInstanceName(Tools::FunctionData& function_data);
        };

    private:
        Users function_users_;
        Groups function_groups_;
        ReadInstanceName struct_read_instance_name_;
        ModifyInstanceName struct_modify_instance_name_;

};

#endif //STRUCTBX_FUNCTIONS_GENERAL_MAIN_H