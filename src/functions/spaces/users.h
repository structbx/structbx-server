
#ifndef STRUCTBI_FUNCTIONS_SPACES_USERS_H
#define STRUCTBI_FUNCTIONS_SPACES_USERS_H


#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBI
{
    namespace Functions
    {
        namespace Spaces
        {
            class Users;
        }
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::Spaces::Users : public Tools::FunctionData
{
    public:
        Users(Tools::FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadUserOutSpace_();
        void Add_();
        /*void Delete_();*/

    private:
        Tools::ActionsData actions_;

};

#endif //STRUCTBI_FUNCTIONS_SPACES_USERS_H