
#ifndef STRUCTBX_FUNCTIONS_ORGANIZATIONS_MAIN_H
#define STRUCTBX_FUNCTIONS_ORGANIZATIONS_MAIN_H


#include "tools/function_data.h"
#include "tools/actions_data.h"

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
        void Read_();
        void Modify_();

    private:
        Tools::ActionsData actions_;
        Users users_;
        Groups groups_;

};

#endif //STRUCTBX_FUNCTIONS_ORGANIZATIONS_MAIN_H