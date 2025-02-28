
#ifndef STRUCTBX_FUNCTIONS_SPACES_USERS_H
#define STRUCTBX_FUNCTIONS_SPACES_USERS_H


#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Spaces
        {
            class Users;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Spaces::Users : public Tools::FunctionData
{
    public:
        Users(Tools::FunctionData& function_data);
        
    protected:
        struct Read : public Tools::FunctionData
        {
            Read(Tools::FunctionData& function_data);
        };
        struct ReadUserOutSpace : public Tools::FunctionData
        {
            ReadUserOutSpace(Tools::FunctionData& function_data);
        };
        struct Add : public Tools::FunctionData
        {
            Add(Tools::FunctionData& function_data);
        };
        struct Delete : public Tools::FunctionData
        {
            Delete(Tools::FunctionData& function_data);
        };

    private:
        Read struct_read_;
        ReadUserOutSpace struct_read_user_out_space_;
        Add struct_add_;
        Delete struct_delete_;
};

#endif //STRUCTBX_FUNCTIONS_SPACES_USERS_H