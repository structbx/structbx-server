

#ifndef STRUCTBX_TOOLS_FUNCTIONDATA
#define STRUCTBX_TOOLS_FUNCTIONDATA

#include <list>

#include "core/nebula_atom.h"
#include "handlers/backend_handler.h"
#include "functions/function.h"
#include "tools/dvalue.h"
#include "tools/hmac_tool.h"
#include "tools/base64_tool.h"

#include "tools/id_checker.h"

namespace StructBX
{
    namespace Tools
    {
        class FunctionData;
    }
}

using namespace StructBX;
using namespace NAF;

class StructBX::Tools::FunctionData
{
    public:
        using FunctionsList = std::shared_ptr<std::list<NAF::Functions::Function::Ptr>>;

        FunctionData() :
            id_user_(-1)
            ,space_id_("")
        {
            functions_ = std::make_shared<std::list<NAF::Functions::Function::Ptr>>();
        }
        FunctionData(FunctionData& function_data) :
            id_user_(function_data.get_id_user())
            ,space_id_(function_data.get_space_id())
            ,functions_(function_data.get_functions())
        {
            
        }

        int get_id_user(){ return id_user_; }
        std::string get_space_id(){ return space_id_; }
        FunctionsList& get_functions()
        {
            auto& var = functions_;
            return var;
        }

        void set_id_user(int id_user){ id_user_ = id_user; }
        void set_space_id(std::string space_id){ space_id_ = space_id; }
        void set_functions(FunctionsList functions){ functions_ = functions; }

    private:
        int id_user_;
        std::string space_id_;
        FunctionsList functions_;
};

#endif //STRUCTBX_TOOLS_FUNCTIONDATA