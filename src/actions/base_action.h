
#ifndef STRUCTBI_PARAMETERS_BASEACTION
#define STRUCTBI_PARAMETERS_BASEACTION

#include "functions/action.h"

#include "function_data.h"

namespace StructBI
{
    namespace Actions
    {
        class BaseAction;
    }
}

using namespace StructBI;
using namespace NAF;

class StructBI::Actions::BaseAction : public FunctionData
{
    public:
        BaseAction(FunctionData& function_data) : 
            FunctionData(function_data)
        {
            action_ = Functions::Action::Ptr(new Functions::Action("a01"));
        }

        Functions::Action::Ptr get_action(){ return action_; };

        void set_action(Functions::Action::Ptr action){ action_ = action; };

        virtual void Setup_(Functions::Action::Ptr action) = 0;

    protected:
        Functions::Action::Ptr action_;
};

#endif //STRUCTBI_PARAMETERS_BASEACTION