
#ifndef STRUCTBI_TOOLS_BASEACTION
#define STRUCTBI_TOOLS_BASEACTION

#include "functions/action.h"

#include "tools/function_data.h"

namespace StructBI
{
    namespace Tools
    {
        class BaseAction;
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Tools::BaseAction : public Tools::FunctionData
{
    public:
        BaseAction(Tools::FunctionData& function_data) : 
            Tools::FunctionData(function_data)
        {
            action_ = Functions::Action::Ptr(new Functions::Action("a01"));
        }

        Functions::Action::Ptr get_action(){ return action_; };

        void set_action(Functions::Action::Ptr action){ action_ = action; };

        virtual void Setup_(Functions::Action::Ptr action) = 0;

    protected:
        Functions::Action::Ptr action_;
};

#endif //STRUCTBI_TOOLS_BASEACTION