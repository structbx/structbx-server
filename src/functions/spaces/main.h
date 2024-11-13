
#ifndef STRUCTBI_FUNCTIONS_SPACES_MAIN_H
#define STRUCTBI_FUNCTIONS_SPACES_MAIN_H


#include "tools/function_data.h"
#include "tools/base64_tool.h"
#include "tools/actions_data.h"

#include "functions/spaces/users.h"

namespace StructBI
{
    namespace Functions
    {
        namespace Spaces
        {
            class Main;
        }
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Functions::Spaces::Main : public Tools::FunctionData
{
    public:
        Main(Tools::FunctionData& function_data);
        
    protected:
        void Read_();
        void ReadSpecific_();
        void Change_();
        void Add_();
        void Modify_();
        void Delete_();

    private:
        Tools::ActionsData actions_;
        Users users_;
};

#endif //STRUCTBI_FUNCTIONS_SPACES_MAIN_H