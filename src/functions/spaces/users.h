
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
        void Read_();
        void ReadUserOutSpace_();
        void Add_();
        void Delete_();

    private:

};

#endif //STRUCTBX_FUNCTIONS_SPACES_USERS_H