
#ifndef STRUCTBX_FUNCTIONS_ORGANIZATIONS_MAIN_H
#define STRUCTBX_FUNCTIONS_ORGANIZATIONS_MAIN_H


#include "tools/base_action.h"
#include "tools/function_data.h"

#include "functions/organizations/users.h"
#include "functions/organizations/groups.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Organizations
        {
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Organizations::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);
        
    protected:
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);
        };
        struct Modify : public Tools::FunctionData
        {
            Modify(Tools::FunctionData& function_data);
        };

    private:
        Users function_users_;
        Groups function_groups_;
        Read struct_read_;
        Modify struct_modify_;

};

#endif //STRUCTBX_FUNCTIONS_ORGANIZATIONS_MAIN_H