
#ifndef STRUCTBI_FUNCTIONS_SPACESUSERS
#define STRUCTBI_FUNCTIONS_SPACESUSERS


#include "tools/function_data.h"
#include "tools/actions_data.h"

namespace StructBI
{
    namespace Functions
    {
        class SpacesUsers;
    }
}

using namespace StructBI;
using namespace NAF;
class StructBI::Functions::SpacesUsers : public Tools::FunctionData
{
    public:
        SpacesUsers(Tools::FunctionData& function_data);
        
    protected:
        void Read_();

    private:
        Tools::ActionsData actions_;

};

#endif //STRUCTBI_FUNCTIONS_SPACESUSERS