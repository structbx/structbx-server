
#ifndef STRUCTBI_ACTIONS_FORMS
#define STRUCTBI_ACTIONS_FORMS

#include "actions/base_action.h"
#include "function_data.h"

namespace StructBI
{
    namespace Actions
    {
        class Forms;
    }
}

using namespace StructBI;

class StructBI::Actions::Forms
{
    public:
        Forms(FunctionData& function_data);

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

        class ReadSpecificA02 : public Actions::BaseAction
        {
            public:
                ReadSpecificA02(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } read_specific_a02_;

        class AddA01 : public Actions::BaseAction
        {
            public:
                AddA01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } add_a01_;

        class AddA02 : public Actions::BaseAction
        {
            public:
                AddA02(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } add_a02_;

        class AddA03 : public Actions::BaseAction
        {
            public:
                AddA03(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } add_a03_;

        class ModifyA01 : public Actions::BaseAction
        {
            public:
                ModifyA01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } modify_a01_;

        class ModifyA02 : public Actions::BaseAction
        {
            public:
                ModifyA02(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } modify_a02_;

        class ModifyA03 : public Actions::BaseAction
        {
            public:
                ModifyA03(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } modify_a03_;

        class Delete01 : public Actions::BaseAction
        {
            public:
                Delete01(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } delete_a01_;

        class Delete02 : public Actions::BaseAction
        {
            public:
                Delete02(FunctionData& function_data);

                virtual void Setup_(Functions::Action::Ptr action) override;

        } delete_a02_;

};

#endif //STRUCTBI_ACTIONS_FORMS