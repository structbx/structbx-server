
#ifndef STRUCTBX_TOOLS_ACTIONSDATA
#define STRUCTBX_TOOLS_ACTIONSDATA

#include "tools/base_action.h"
#include "tools/function_data.h"

namespace StructBX
{
    namespace Tools
    {
        class ActionsData;
    }
}

using namespace StructBX;

class StructBX::Tools::ActionsData
{
    public:
        ActionsData(Tools::FunctionData& function_data);

        /*
        struct ElementName
        {
            ElementName(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;
        } element_name_;
        */

};

#endif //STRUCTBX_TOOLS_ACTIONSDATA