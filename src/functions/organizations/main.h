
#ifndef STRUCTBI_FUNCTIONS_ORGANIZATIONS_MAIN_H
#define STRUCTBI_FUNCTIONS_ORGANIZATIONS_MAIN_H


#include "tools/function_data.h"
#include "tools/actions_data.h"

#include "functions/organizations/users.h"

namespace StructBI
{
    namespace Functions
    {
        namespace Organizations
        {
            class Main;
        }
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::Organizations::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);
        
    protected:
        void Read_();
        void Modify_();

    private:
        Tools::ActionsData actions_;
        Users users_;

};

#endif //STRUCTBI_FUNCTIONS_ORGANIZATIONS_MAIN_H