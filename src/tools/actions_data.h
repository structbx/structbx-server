
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

        struct FormsColumns
        {
            FormsColumns(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
                ,read_specific_a01_(function_data)
                ,read_types_a01_(function_data)
                ,add_a01_(function_data)
                ,add_a02_(function_data)
                ,add_a03_(function_data)
                ,modify_a01_(function_data)
                ,modify_a02_(function_data)
                ,modify_a03_(function_data)
                ,delete_a01_(function_data)
                ,delete_a02_(function_data)
                ,delete_a03_(function_data)
            {

            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;

            class ReadSpecificA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_specific_a01_;

            class ReadTypesA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_types_a01_;

            class AddA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a01_;

            class AddA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a02_;

            class AddA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } add_a03_;

            class ModifyA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a01_;

            class ModifyA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a02_;

            class ModifyA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } modify_a03_;

            class DeleteA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a01_;

            class DeleteA02 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a02_;

            class DeleteA03 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } delete_a03_;

        } forms_columns_;

        struct FormsPermissions
        {
            FormsPermissions(Tools::FunctionData& function_data) : 
                read_a01_(function_data)
            {
                
            }

            class ReadA01 : public Tools::BaseAction
            {
                public:
                    using Tools::BaseAction::BaseAction;

                    virtual void Setup_(Functions::Action::Ptr action) override;

            } read_a01_;
        } forms_permissions_;
};

#endif //STRUCTBX_TOOLS_ACTIONSDATA