
#ifndef STRUCTBX_FUNCTIONS_SPACES_USERS_H
#define STRUCTBX_FUNCTIONS_SPACES_USERS_H


#include "tools/function_data.h"
#include "tools/actions_data.h"

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
        Tools::ActionsData actions_;

};

#endif //STRUCTBX_FUNCTIONS_SPACES_USERS_H