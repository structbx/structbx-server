
#ifndef STRUCTBI_ACTIONS_SPACESDATA
#define STRUCTBI_ACTIONS_SPACESDATA

#include "actions/base_action.h"
#include "function_data.h"

namespace StructBI
{
    namespace Actions
    {
        class SpacesData;
    }
}

using namespace StructBI;

class StructBI::Actions::SpacesData
{
    public:
        SpacesData(FunctionData& function_data);

        class ReadA01 : public Actions::BaseAction
        {
            public:
                ReadA01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } read_a01_;


        class ReadSpecificA01 : public Actions::BaseAction
        {
            public:
                ReadSpecificA01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } read_specific_a01_;

        class ChangeA01 : public Actions::BaseAction
        {
            public:
                ChangeA01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } change_a01_;

        class ModifyA01 : public Actions::BaseAction
        {
            public:
                ModifyA01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;
                
        } modify_a01_;
};

#endif //STRUCTBI_ACTIONS_SPACESDATA