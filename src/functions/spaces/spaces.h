
#ifndef STRUCTBI_FUNCTIONS_SPACES
#define STRUCTBI_FUNCTIONS_SPACES


#include "tools/function_data.h"
#include "tools/base64_tool.h"
#include "tools/actions_data.h"

namespace StructBI
{
    namespace Functions
    {
        class Spaces;
    }
}

using namespace StructBI;
using namespace NAF;
class StructBI::Functions::Spaces : public Tools::FunctionData
{
    public:
        Spaces(Tools::FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadSpecific_();
        void Change_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;

};

#endif //STRUCTBI_FUNCTIONS_SPACES