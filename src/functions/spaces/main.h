
#ifndef STRUCTBX_FUNCTIONS_SPACES_MAIN_H
#define STRUCTBX_FUNCTIONS_SPACES_MAIN_H

#include "Poco/DirectoryIterator.h"

#include "tools/function_data.h"
#include "tools/base64_tool.h"
#include "tools/actions_data.h"

#include "functions/spaces/users.h"

namespace StructBX
{
    namespace Functions
    {
        namespace Spaces
        {
            class Main;
        }
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Functions::Spaces::Main : public Tools::FunctionData
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

#endif //STRUCTBX_FUNCTIONS_SPACES_MAIN_H